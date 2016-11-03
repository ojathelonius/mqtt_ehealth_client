// Includes
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"
#include "mqtt/mqtt_client.h"

// Définition des constantes globales
const std::string ADDRESS("tcp://localhost:1883"); // à changer pour y mettre l'adresse du broker
const std::string CLIENTID("AsyncPublisher");
const std::string TOPIC("ehealth");

const int QOS = 1;
const long TIMEOUT = 10000L;

class callback : public virtual mqtt::callback
{
public:
	virtual void connection_lost(const std::string& cause) {
		std::cout << "\nConnexion interrompue" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {}

	virtual void delivery_complete(mqtt::idelivery_token_ptr tok) {
		std::cout << "Livraison terminée "
			<< (tok ? tok->get_message_id() : -1) << std::endl;
	}
};

class action_listener : public virtual mqtt::iaction_listener
{
protected:
	virtual void on_failure(const mqtt::itoken& tok) {
		std::cout << "\n\tListener: échec sur le token: "
			<< tok.get_message_id() << std::endl;
	}

	virtual void on_success(const mqtt::itoken& tok) {
		std::cout << "\n\tListener: succès sur le token : "
			<< tok.get_message_id() << std::endl;
	}
};


class delivery_action_listener : public action_listener
{
	bool done_;

	virtual void on_failure(const mqtt::itoken& tok) {
		action_listener::on_failure(tok);
		done_ = true;
	}

	virtual void on_success(const mqtt::itoken& tok) {
		action_listener::on_success(tok);
		done_ = true;
	}

public:
	delivery_action_listener() : done_(false) {}
	bool is_done() const { return done_; }
};

int envoyerVersBroker(int data){
  std::string data_string = std::to_string(data);
  char const* data_char = data_string.c_str();
  mqtt::async_client client(ADDRESS, CLIENTID);

  callback cb;
  client.set_callback(cb);

  try {

    mqtt::itoken_ptr conntok = client.connect();
    std::cout << "En attente de connexion..." << std::flush;
    conntok->wait_for_completion();
    std::cout << "OK" << std::endl;

    std::cout << "Envoi du message..." << std::flush;
    mqtt::idelivery_token_ptr pubtok;
    pubtok = client.publish(TOPIC, data_char, std::strlen(data_char), QOS, false);
    pubtok->wait_for_completion(TIMEOUT);
    std::cout << "OK" << std::endl;

    // Tokens restants

    std::vector<mqtt::idelivery_token_ptr> toks = client.get_pending_delivery_tokens();
    if (!toks.empty())
      std::cout << "Erreur : tokens restant à envoyer" << std::endl;

    // Déconnexion
    std::cout << "Déconnexion..." << std::flush;
    conntok = client.disconnect();
    conntok->wait_for_completion();
    std::cout << "OK" << std::endl;
  }
  catch (const mqtt::exception& exc) {
    std::cerr << "Erreur: " << exc.what() << std::endl;
    return 1;
  }

  return 0;
}
