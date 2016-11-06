// Includes
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <ctime>
#include <memory>
#include "mqtt/async_client.h"
#include "mqtt/mqtt_client.h"
#include "mqtt/mqtt_client_config.h"


class callback : public virtual mqtt::callback {
public:

    virtual void connection_lost(const std::string& cause) {
        std::cout << "\nConnection interrupted" << std::endl;

        if (!cause.empty()) std::cout << "\tcause: " << cause << std::endl;
    }

    virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {}

    virtual void delivery_complete(mqtt::idelivery_token_ptr tok) {
        std::cout << "Delivery complete ! "
                  << (tok ? tok->get_message_id() : -1) << std::endl;
    }
};

class action_listener : public virtual mqtt::iaction_listener {
protected:

    virtual void on_failure(const mqtt::itoken& tok) {
        std::cout << "\n\tListener: token failure "
                  << tok.get_message_id() << std::endl;
    }

    virtual void on_success(const mqtt::itoken& tok) {
        std::cout << "\n\tListener: token success "
                  << tok.get_message_id() << std::endl;
    }
};


class delivery_action_listener : public action_listener {
    bool done_;

    virtual void on_failure(const mqtt::itoken& tok) {
        action_listener::on_failure(tok);
        done_ = true;
    }

    virtual void on_success(const mqtt::itoken& tok) {
        action_listener::on_success(tok);
        done_ = true;
    }

public:

    delivery_action_listener() : done_(false) {}

    bool is_done() const {
        return done_;
    }
};


mqtt::iasync_client* connectTo() {
    mqtt::async_client client("tcp://" + mqtt_client_config.host_address + ":" + std::to_string(mqtt_client_config.port), mqtt_client_config.client_id);

    callback cb;
    mqtt::itoken_ptr conntok;
    mqtt::iasync_client* client_return;
    client.set_callback(cb);

    try {
        mqtt::connect_options options;
        options.set_user_name(mqtt_client_config.user);
        options.set_password(mqtt_client_config.password);
        conntok = client.connect(options);
        client_return = conntok->get_client();
        std::cout << "Awaiting connection..." << std::flush;
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        // return client_return;
    }
    return client_return;
}

bool disconnectFrom(mqtt::iasync_client* client) {
    mqtt::itoken_ptr conntok;
    std::cout << "Disconnecting..." << std::flush;
    try {
        conntok = client->disconnect();
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return true;
    }
    return false;
}

bool sendInteger(int data, std::string type, bool with_timestamp) {
    // Final JSON output looks like the following :
    // { type : "type", data : 433, client_id : "client_id", timestamp : 1478294310 }
    std::string data_string = std::to_string(data);
    std::string string_json;

    if (with_timestamp) {
        string_json = "{ type : \"" + type + "\", data : " + data_string +", client_id : \""+mqtt_client_config.client_id+"\", timestamp : " + std::to_string(std::time(0)) + " }";
    }
    else
    {
        string_json = "{ type : \"" + type + "\", data : " + data_string + " }";
    }
    char const *char_json = string_json.c_str();
    mqtt::async_client client("tcp://" + mqtt_client_config.host_address + ":" + std::to_string(mqtt_client_config.port), mqtt_client_config.client_id);

    callback cb;

    client.set_callback(cb);

    try {
        mqtt::connect_options options;
        options.set_user_name(mqtt_client_config.user);
        options.set_password(mqtt_client_config.password);
        mqtt::itoken_ptr conntok = client.connect(options);
        std::cout << "Awaiting connection..." << std::flush;
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;

        std::cout << "Sending message..." << std::flush;
        mqtt::idelivery_token_ptr pubtok;
        pubtok = client.publish(mqtt_client_config.topic, char_json, std::strlen(char_json), mqtt_client_config.qos, false);
        pubtok->wait_for_completion(mqtt_client_config.timeout);
        std::cout << "OK" << std::endl;

        // Remaining tokens
        std::vector<mqtt::idelivery_token_ptr> toks = client.get_pending_delivery_tokens();

        if (!toks.empty()) std::cout << "Error : pending delivery tokens remaining" << std::endl;

        // Disconnecting
        std::cout << "Disconnecting..." << std::flush;
        conntok = client.disconnect();
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return true;
    }
    return false;
}

std::shared_ptr<mqtt::itoken>sendString(std::string data, std::string type, bool with_timestamp) {
    // Final JSON output looks like the following :
    // 	{ type : "string_type", data : "some string", client_id : "client_id", timestamp : 1478294311 }
		std::string string_json;
    if (with_timestamp) {
        string_json = "{ type : \"" + type + "\", data : \"" + data + ", client_id : \""+mqtt_client_config.client_id+"\", timestamp : " + std::to_string(std::time(0)) + " }";
    }
    else
    {
        string_json = "{ type : \"" + type + "\", data : \"" + data + "\" }";
    }

    char const *char_json = string_json.c_str();
    mqtt::async_client client("tcp://" + mqtt_client_config.host_address + ":" + std::to_string(mqtt_client_config.port), mqtt_client_config.client_id);

    callback cb;
    mqtt::itoken_ptr conntok;
    client.set_callback(cb);

    try {
        mqtt::connect_options options;
        options.set_user_name(mqtt_client_config.user);
        options.set_password(mqtt_client_config.password);
        conntok = client.connect(options);
        std::cout << "Awaiting connection..." << std::flush;
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;

        std::cout << "Sending message..." << std::flush;
        mqtt::idelivery_token_ptr pubtok;
        pubtok = client.publish(mqtt_client_config.topic, char_json, std::strlen(char_json), mqtt_client_config.qos, false);
        pubtok->wait_for_completion(mqtt_client_config.timeout);
        std::cout << "OK" << std::endl;

        // Remaining tokens

        std::vector<mqtt::idelivery_token_ptr> toks = client.get_pending_delivery_tokens();

        if (!toks.empty()) std::cout << "Error : pending delivery tokens remaining" << std::endl;

        // Disconnecting
        std::cout << "Disconnecting..." << std::flush;
        conntok = client.disconnect();
        conntok->wait_for_completion();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return conntok;
    }
    return conntok;
}
