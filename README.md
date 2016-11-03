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
* Copier les fichiers
```
cd paho.mqtt.cpp/lib
sudo cp -r . /usr/local/lib
```

* Rafraîchir la liste des liens dynamiques
```
sudo ldconfig
```

### Récupérer le client 
```
git clone https://github.com/ojathelonius/mqtt_ehealth_client.git
```

### Renseigner l'IP du broker
```
cd mqtt_ehealth_client/lib/
nano mqtt_client.cpp
```

### Compiler le fichier airflow.cpp
```
sudo make
```
A l'ajout d'autres fichiers C++, il faut les ajouter dans le fichier Makefile dans all et clean

### Lancement
```
./airflow
```

### Tester avec Mosquitto
```
sudo apt-get install mosquitto-dev
sudo service mosquitto start
mosquitto_sub -h adresse_du_broker -t topic
```
Attention, si vous publiez sur l'adresse du broker il faut lancer mosquitto_sub avant de lancer airflow !
L'adresse du broker et le nom du topic sont actuellement à modifier dans mqtt_client.cpp .


Problème : impossible actuellement d'authentifier l'utilisateur via login/mot de passe : problème de type ?

```
const std::string USER("nom_user");
const std::string PASSWORD("mdp_user");

// Lors de la connexion au broker, remplacer par :
mqtt::connect_options options;
options.set_user_name(USER);
options.set_password(PASSWORD);
mqtt::itoken_ptr conntok = client.connect(options);
```
