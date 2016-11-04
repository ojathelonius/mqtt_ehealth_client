#ifndef MQTT_CLIENT_CONFIG
#define MQTT_CLIENT_CONFIG
#include <string>

extern struct Mqtt_client_config mqtt_client_config;
struct Mqtt_client_config {
        std::string host_address;
        int port;
        std::string topic;
        std::string client_id;
        std::string user;
        std::string password;
        int delay;
        int qos;
        long timeout;
};

void loadConfig();

#endif
