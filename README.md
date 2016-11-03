# mqtt_ehealth_client

## Getting started
### Installer openSSL
```
sudo apt-get install libssl-dev
```
### Installer le client Paho MQTT en C
```
git clone https://github.com/eclipse/paho.mqtt.c.git
sudo make install
```
### Installer le client Paho MQTT en C++
```
git clone https://github.com/eclipse/paho.mqtt.cpp.git
sudo make
```
### Ajouter manuellement les librairies dans /usr/local/lib/ 
* Dans /home/pi/paho.mqtt.cpp/src/samples
```
cd home/pi/paho.mqtt.cpp/lib
sudo cp -r . /usr/local/lib
sudo ldconfig // pour rafraîchir la liste des liens dynamiques
```
