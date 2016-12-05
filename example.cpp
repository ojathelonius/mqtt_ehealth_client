// EXAMPLE USING airflow.cpp WITH FAKE DATA

// In order to actually work with real sensor data, this example needs to be run on a Raspberry Pi with bcm2835 installed (see on
// Cooking-Hacks)

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include "lib/mqtt/mqtt_client.h"
#include "lib/mqtt/mqtt_client_config.h"

// Enable below to make it work on Raspberry
// #include "eHealth.h"

Mqtt_client_config mqtt_client_config;

inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void loop();

// Getting random data for demo purposes
int  getAirFlow() {
    return rand() % (100 + 1) + 100;
}

void loop(mqtt::iasync_client& client) {
    // Enable below to make it work on the Raspberry, using airflow data
    // int air = eHealth.getAirFlow();
    // eHealth.airFlowWave(air);
    int air = getAirFlow();
    int pulsio= getAirFlow();
    int blood = getAirFlow();
    // Syntax is the following : sendFunction(client, data, "type", true);
    sendInteger(client, air, "airflow", true);
    sendInteger(client, pulsio, "pulsiometer", true);
    sendInteger(client, blood, "bloodpressure", true);

    sleep(mqtt_client_config.delay);
}

// void sendToBroker(){};

int main() {
    // Loading configuration file
    loadConfig();

    // Creating the client item and connecting
    mqtt::async_client client("tcp://" + mqtt_client_config.host_address + ":" + std::to_string(mqtt_client_config.port), mqtt_client_config.client_id);

    if (!client.is_connected()) {
        connectTo(client);
    }

    while (1) {
        loop(client);
    }

    disconnectFrom(client);
    return 0;
}
