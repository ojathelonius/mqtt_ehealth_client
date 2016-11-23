# mqtt_ehealth_client
Integrating the e-health sensors functions in the Paho C++ client for MQTT in order to send data to a remote broker on a Raspberry Pi. This fixes most issues that I found trying to deal with the C++ Paho library.
All this project does is retrieve data from the sensors and send it to a broker, using the airflow.cpp example (with fake data in order to work on any device).

See https://github.com/ojathelonius/mqtt_ehealth_subscriber to have an overview of how to subscribe to the broker using NodeJS.

Important note : the **master branch** allows **simulating data** received from eHealth sensors. The **dev branch** allows you to receive **actual data** from the sensors, thus includes eHealth and arduPi libraries. Please check out the dev branch if you want to make it work on a Raspberry.

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
Modify the mqtt_client_config.JSON file to add your broker IP, credentials and options


### Compile a sensor C++ file (e.g. airflow.cpp)
```
sudo make
```
Note : if you add sensor files, make sure you also add them in the Makefile (add & clean)


### Start retrieving sensor data and sending it to the broker
```
./example
```
See https://www.cloudmqtt.com/ for a free hosted broker. Or see below to learn how to host your own mosquitto broker (on a VPS for instance).

### Host your own Mosquitto broker
* Install Mosquitto (on your VPS)
```
apt-get update
apt-get install build-essential libwrap0-dev libssl-dev libc-ares-dev uuid-dev xsltproc
sudo apt-get install mosquitto
```
* Create a new user and store its credentials in a password file
```
mosquitto_passwd -c /etc/mosquitto/pwfile
```
* Create a new configuration file from the example
```
cp /etc/mosquitto/mosquitto.conf.example /etc/mosquitto/mosquitto.conf
nano /etc/mosquitto/mosquitto.conf
```
* Add your own info in mosquitto.conf
```
listener 1883 <VPS_IP>
connection_messages true
log_timestamp true
allow_anonymous false
password_file /etc/mosquitto/pwfile
```
* Update links
```
/sbin/ldconfig
```
* Start the broker in the background
```
mosquitto -c -d /etc/mosquitto/mosquitto.conf
```

### Test it 
* If you didn't install your own broker
```
sudo apt-get install mosquitto-dev
sudo service mosquitto start
mosquitto_sub -h 127.0.0.1 -t <topic>
```
* With your own broker
```
mosquitto_sub -h <broker_adress> -P <port> -t <topic> -u <user> -p <password> 
```
