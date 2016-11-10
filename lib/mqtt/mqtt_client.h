#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include <memory>


extern "C" {
	#include "MQTTAsync.h"
}

#include "async_client.h"
#include "token.h"

bool sendInteger(mqtt::iasync_client &client, int data, std::string type, bool with_timestamp);
bool disconnectFrom(mqtt::iasync_client &client);
std::shared_ptr<mqtt::itoken> sendString(std::string data, std::string type, bool with_timestamp);
void connectTo(mqtt::iasync_client &client);

#endif
