#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

bool sendInteger(int data, std::string type, bool with_timestamp);
bool sendString(std::string data, std::string type, bool with_timestamp);

#endif
