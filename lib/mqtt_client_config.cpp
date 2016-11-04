#include "mqtt/mqtt_client_config.h"
#include "json/json.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;


string host_address;
int port;
string topic;
string client_id;
string user;
string password;
int delay;
int qos;
long timeout;

void loadConfig() {
        Json::Value root; // will contains the root value after parsing.
        Json::Reader reader;
        ifstream file("mqtt_client_config.json");
        bool parsingSuccessful = reader.parse(file, root );
        if ( !parsingSuccessful )
        {
                // report to the user the failure and their locations in the document.
                std::cout << "Failed to parse MQTT client configuration file\n"
                          << reader.getFormattedErrorMessages();
                return;
        }
        else{
          // Host informations
          mqtt_client_config.host_address = root["host_info"]["host_address"].asString();
          mqtt_client_config.port = root["host_info"]["port"].asInt();
          // User credentials
          mqtt_client_config.user = root["user_credentials"]["user"].asString();
          mqtt_client_config.password = root["user_credentials"]["password"].asString();
          mqtt_client_config.client_id = root["user_credentials"]["client_id"].asString();
          // Options
          mqtt_client_config.topic = root["options"]["topic"].asString();
          mqtt_client_config.delay = root["options"]["delay"].asInt();
          mqtt_client_config.qos = root["options"]["qos"].asInt();
          mqtt_client_config.timeout = stol(root["options"]["timeout"].asString());
        }

        // TODO : retrieve an array of topics from the configuration file

}
