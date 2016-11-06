#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include <memory>


extern "C" {
	#include "MQTTAsync.h"
}

#include "async_client.h"
#include "token.h"

bool sendInteger(int data, std::string type, bool with_timestamp);
bool disconnectFrom(mqtt::iasync_client* client);
std::shared_ptr<mqtt::itoken> sendString(std::string data, std::string type, bool with_timestamp);
mqtt::iasync_client* connectTo();

#endif
