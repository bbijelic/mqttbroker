/*
 * MqttServer.hpp
 *
 *  Created on: Dec 9, 2016
 *      Author: dev
 */

#ifndef MQTT_MQTTSERVER_HPP_
#define MQTT_MQTTSERVER_HPP_

#include "Epoll.hpp"
#include "ServerConfiguration.hpp"

#include <vector>

namespace MQTT {

using namespace Configuration;
using namespace Notification;

/**
 * Mqtt class
 */
class MqttServer {
private:

	/**
	 * Server configuration
	 */
	ServerConfiguration* m_config;


	/**
	 * Epoll
	 */
	Epoll* m_epoll;

public:

	/**
	 * Constructor
	 */
	MqttServer(ServerConfiguration* config, Epoll* epoll) :
			m_config(config), m_epoll(epoll) {};

	/**
	 * Destructor
	 */
	~MqttServer();

	/**
	 * Starts server
	 */
	void startServer();

	/**
	 * Stops server
	 */
	void stopServer();

};

}

#endif /* MQTT_MQTTSERVER_HPP_ */
