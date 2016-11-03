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
```
* Rafraîchir la liste des liens dynamiques
```
sudo ldconfig
```

### Compiler le fichier airflow.cpp
```
sudo make
```
A l'ajout d'autres fichiers C++, il faut les ajouter dans le fichier Makefile dans all, async_publish et clean

### Tester avec Mosquitto
```
sudo apt-get install mosquitto-dev
sudo service mosquitto start
mosquitto_sub -h adresse_du_broker -t topic
```
L'adresse du broker et le nom du topic sont à modifier dans les fichiers C++
