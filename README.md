# mqtt_ehealth_client
Integrating the e-health sensors functions in the Paho C++ client for MQTT in order to send data to a remote broker on a Raspberry Pi. This fixes most issues that I found trying to deal with the C++ Paho library.
All this project does is retrieve data from the sensors and send it to a broker, using the airflow.cpp example (with fake data in order to work on any device).

As a school project, I'm also temporarily going to list everything that I had to do in order to set up my VPS, which now hosts the broker, the MongoDB database, the restAPI as well as the website. It may then move to a Gist later on.

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
mosquitto -c /etc/mosquitto/mosquitto.conf -d 
```

### Test it 
* If you didn't install your own broker
```
sudo apt-get install mosquitto-dev
sudo apt-get install mosquitto-clients
sudo service mosquitto start
mosquitto_sub -h 127.0.0.1 -t <topic>
```
* With your own broker
```
mosquitto_sub -h <broker_adress> -P <port> -t <topic> -u <user> -p <password> 
```
### Allow WebSocket in Mosquitto to display real-time data in a browser
The Paho Javascript library allows you to retrieve real-time data from the broker using WebSockets. However mosquitto 1.4.10, which is available through apt-get, doesn't allow websockets. We need to build mosquitto 1.4.x (using Mosquitto 1.4.2 here) manually.
* Install libwebsockets
We prepare the build system.
```
sudo apt-get update
sudo apt-get install build-essential python quilt devscripts python-setuptools python3
sudo apt-get install libssl-dev
sudo apt-get install cmake
sudo apt-get install libc-ares-dev
sudo apt-get install uuid-dev
sudo apt-get install daemon
```
Then we download, build and install libwebsockets.
```
wget http://git.libwebsockets.org/cgi-bin/cgit/libwebsockets/snapshot/libwebsockets-1.4-chrome43-firefox-36.tar.gz
tar zxvf libwebsockets-1.4-chrome43-firefox-36.tar.gz
cd libwebsockets-1.4-chrome43-firefox-36
mkdir build
cd build
cmake ..
sudo make install
sudo ldconfig
```
* Installing Mosquitto 1.4.2
```
wget http://mosquitto.org/files/source/mosquitto-1.4.2.tar.gz
tar zxvf mosquitto-1.4.2.tar.gz
cd mosquitto-1.4.2
```
Modify **config.mk** by replacing WITH_WEBSOCKETS:=no with WITH_WEBSOCKETS:=yes , then :
```
make
sudo make install
```
* Adding the WebSocket port in **mosquitto.conf**
```
nano /etc/mosquitto/mosquitto.conf
```
Modify **mosquitto.conf** by adding the two following lines :
```
port 1884
protocol websockets
```
Make sure you keep the default port (1883) if you want to keep subscribing with your other MQTT clients.
* Restart the mosquitto service
```
sudo service mosquitto stop
mosquitto -c /etc/mosquitto/mosquitto.conf -d 
```
Et voilà !

### Host several NodeJS apps with various subdomain names on the same VPS
* Create DNS entries for each subdomains (e.g. app.domain.com, api.domain.com ) in your hosting service manager
* Install nginx on your VPS
```
sudo apt-get install nginx
```
* Create as many configuration files as NodeJS apps you want to run
```
nano /etc/nginx/conf.d/app1.conf
nano /etc/nginx/conf.d/app2.conf
```
* And use the following template for each :
```
server {
    listen 80;
 
    server_name subdomain_you_want_to_use;
 
    location / {
        proxy_pass http://localhost:<port_of_the_app>;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection 'upgrade';
        proxy_set_header Host $host;
        proxy_cache_bypass $http_upgrade;
    }
}
```
* Restart nginx service
```
sudo service nginx restart
```
* Start all those apps with forever. All done !


