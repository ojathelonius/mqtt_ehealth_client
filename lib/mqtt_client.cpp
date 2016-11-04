// Includes
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"
#include "mqtt/mqtt_client.h"
#include "mqtt/mqtt_client_config.h"


class callback : public virtual mqtt::callback
{
public:
	virtual void connection_lost(const std::string& cause) {
		std::cout << "\nConnection interrupted" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {}

	virtual void delivery_complete(mqtt::idelivery_token_ptr tok) {
		std::cout << "Delivery complete ! "
			<< (tok ? tok->get_message_id() : -1) << std::endl;
	}
};

class action_listener : public virtual mqtt::iaction_listener
{
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


class delivery_action_listener : public action_listener
{
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
	bool is_done() const { return done_; }
};

bool sendInteger(int data){
  std::string data_string = std::to_string(data);
  char const* data_char = data_string.c_str();
  mqtt::async_client client("tcp://"+mqtt_client_config.host_address+":"+std::to_string(mqtt_client_config.port), mqtt_client_config.client_id);

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
    pubtok = client.publish(mqtt_client_config.topic, data_char, std::strlen(data_char), mqtt_client_config.qos, false);
    pubtok->wait_for_completion(mqtt_client_config.timeout);
    std::cout << "OK" << std::endl;

    // Remaining tokens

    std::vector<mqtt::idelivery_token_ptr> toks = client.get_pending_delivery_tokens();
    if (!toks.empty())
      std::cout << "Error : pending delivery tokens remaining" << std::endl;

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
