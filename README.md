# mqtt_ehealth_client
Integrating the e-health sensors functions in the Paho C++ client for MQTT in order to send data to a remote broker on a Raspberry Pi. This fixes most issues that I found trying to deal with the C++ Paho library.
All this project does is retrieving data from the sensors and sending it to a broker.

## Getting started
### Install openSSL
```
sudo apt-get install libssl-dev
```

### Install Paho C client
```
git clone https://github.com/eclipse/paho.mqtt.c.git
sudo make install
```

### Install Paho C++ client
```
git clone https://github.com/eclipse/paho.mqtt.cpp.git
sudo make
```

### Manually adding lib files in /usr/local/lib/ 
* Copying files
```
cd paho.mqtt.cpp/lib
sudo cp -r . /usr/local/lib
```

* Updating ld
```
sudo ldconfig
```

### Cloning the client on the Raspberry
```
git clone https://github.com/ojathelonius/mqtt_ehealth_client.git
```

### Modify the mqtt_client file to add your own broker, credentials and topic
```
cd mqtt_ehealth_client/lib/
nano mqtt_client.cpp
```

### Compile a sensor C++ file (e.g. airflow.cpp)
```
sudo make
```
Note : if you add sensor files, make sure you also add them in the Makefile (add & clean)

### Start retrieving sensor data and sending it to the broker
```
./airflow
```
See https://www.cloudmqtt.com/ for a free hosted broker

### Test it using Mosquitto
```
sudo apt-get install mosquitto-dev
sudo service mosquitto start
mosquitto_sub -h <broker_adress> -t <topic>
```
Make sure you start mosquitto_sub before sending any data.

