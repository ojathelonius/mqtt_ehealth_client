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
#include "lib/eHealth/eHealth.h"

Mqtt_client_config mqtt_client_config;
int cont = 0;

void readPulsioximeter();
void loop();

void setup() {
 eHealth.initPulsioximeter();
 attachInterrupt(6, readPulsioximeter, RISING);
}

inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}



void loop(mqtt::iasync_client& client) {
    int air = eHealth.getAirFlow();
    int bpm = eHealth.getBPM();
    float temp = eHealth.getTemperature();
    float conductance = eHealth.getSkinConductance();
    float resistance = eHealth.getSkinResistance();
    // Absurd values are ignored
    if(air != 0){
         sendInteger(client, air, "airflow", true);
    }
    if(temp != 0 && temp > 33){
         sendFloat(client, temp, "temperature", true);
    }
    if(conductance > 0 && conductance < 8){
         sendFloat(client, conductance, "conductance", true);
    }
    if(resistance < 1000000 && resistance > 0){
        sendFloat(client, resistance, "resistance", true);
    }

    sleep(mqtt_client_config.delay);
}

void readPulsioximeter(){
    cont ++;
    if (cont == 50) { //Get only one 50 measures to reduce the latency
        eHealth.readPulsioximeter();
        cont = 0;
    }
}

int main() {
    // Loading configuration file
    loadConfig();

    // Set up sensors

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
