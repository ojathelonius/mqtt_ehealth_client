#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include <memory>


extern "C" {
	#include "MQTTAsync.h"
}

#include "async_client.h"
#include "token.h"

void sendInteger(mqtt::iasync_client &client, int data, std::string type, bool with_timestamp);
void disconnectFrom(mqtt::iasync_client &client);
void connectTo(mqtt::iasync_client &client);

#endif
