// EXAMPLE USING airflow.cpp WITH FAKE DATA
// In order to actually work with real sensor data, this example needs to be run on a Raspberry Pi with bcm2835 installed (see on Cooking-Hacks)

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include "lib/mqtt/mqtt_client.h"
// Enable below to make it work on Raspberry
// #include "eHealth.h"

const int DELAY = 50;

inline void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Getting random data for demo purposes
int getAirFlow(){
  return rand()%(100 + 1) + 100;
}

void loop() {
	// Enable below to make it work on the Raspberry, using airflow data
	// int air = eHealth.getAirFlow();
	// eHealth.airFlowWave(air);
	int air = getAirFlow();
  sendInteger(air);
	sleep(DELAY);
}

int main (){
	while(1){
		loop();
	}
	return (0);
}
