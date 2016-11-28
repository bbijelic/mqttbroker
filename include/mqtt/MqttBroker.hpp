/*
 * MqttBroker.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef MQTT_MQTTBROKER_HPP_
#define MQTT_MQTTBROKER_HPP_

#include <string>

#include <TcpConnectionAcceptor.hpp>
#include "TcpConnectionQueue.hpp"
#include "TcpConnectionHandler.hpp"
#include "TcpConnection.hpp"

using namespace TCP;
using namespace std;

namespace MQTT {

/**
 * MQTT Broker class
 */
class MqttBroker {
private:

	/**
	 * Number of workers
	 */
	int m_workers;

	/**
	 * Port number
	 */
	int m_port;

	/**
	 * Ip to bind to
	 */
	string m_bind_ip;

	/**
	 * TCP connection queue
	 */
	TcpConnectionQueue<TcpConnectionQueueItem*> m_queue;

	/**
	 * TCP connection acceptor
	 */
	TcpConnectionAcceptor* m_tcp_connection_acceptor;

	/**
	 * Starts TCP connection handlers
	 */
	void startTcpConnectionHandlers();

	/**
	 * Starts TCP connection acceptor
	 */
	void startTcpConnectionAcceptor();

	/**
	 * Starts accepting TCP connections
	 */
	void startAcceptingConnections();

public:

	/**
	 * Constructor
	 */
	MqttBroker(int workers, int port, string bind_ip);

	/**
	 * Starts broker
	 */
	void start();

	/**
	 * Destructor
	 */
	~MqttBroker();

};

}


#endif /* MQTT_MQTTBROKER_HPP_ */
