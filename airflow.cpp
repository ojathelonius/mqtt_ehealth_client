#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include "lib/mqtt/mqtt_client.h"

const int DELAY = 50;

inline void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
// Récupération factice de data
int getAirFlow(){
  return rand()%(100 + 1) + 100;
}

void loop() {
  int maData = getAirFlow();
  envoyerVersBroker(maData);
	sleep(DELAY);
}

int main (){
	while(1){
		loop();
	}
	return (0);
}
