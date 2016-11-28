/*
 * MqttBroker.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include "easylogging++.hpp"
#include "MqttBroker.hpp"

using namespace TCP;
using namespace MQTT;

MqttBroker::MqttBroker(int workers, int port, string bind_ip) :
		m_workers(workers), m_port(port), m_bind_ip(bind_ip) {

}

void MqttBroker::startTcpConnectionHandlers() {
	LOG(INFO)<< "Initializing TCP connection handlers: " << m_workers;

	// Initialize TCP connection queue handlers
	for (int i = 0; i < m_workers; i++) {
		// Instance of the TCP connection handler
		TcpConnectionHandler *connection_handler = new TcpConnectionHandler(
				m_queue);

		// Checking if TCP connection handler initialized
		if (!connection_handler) {
			LOG(ERROR)<< "Failed to initialize TCP connection handler: " << i;
			exit(-1);
		}

		// Start the connection handler
		connection_handler->start();
	}
}

void MqttBroker::startTcpConnectionAcceptor() {
	LOG(INFO)<< "Starting TCP connection acceptor";

	if(m_bind_ip.length() > 0) {
		LOG(INFO) << "Binding to the IP '" << m_bind_ip << "' on port " << m_port;

		// Initialize TCP connection acceptor
		m_tcp_connection_acceptor = new TcpConnectionAcceptor(m_port, (char*)m_bind_ip.c_str());

	} else {
		// IP address not defined
		// Binds to all interfaces on a port
		m_tcp_connection_acceptor = new TcpConnectionAcceptor(m_port);
	}

	if(!m_tcp_connection_acceptor || m_tcp_connection_acceptor->start() != 0){
		LOG(ERROR) << "Could not start TCP connection acceptor";
		exit(-1);
	}
}

void MqttBroker::startAcceptingConnections(){
	LOG(INFO) << "Starting to accept incoming connections";

	// Initialize TCP connection queue item
	TcpConnectionQueueItem* tcp_connection_queue_item;

	while(true){
		// Accept TCP connection
		TcpConnection* tcp_connection = m_tcp_connection_acceptor->accept();

		if(!tcp_connection){
			LOG(ERROR) << "Could not accept connection";
			continue;
		}

		LOG(INFO) << "Connection from host '" << tcp_connection->getPeerIp() << "' accepted";

		// Instance of TCP connection queue item
		tcp_connection_queue_item = new TcpConnectionQueueItem(tcp_connection);

		if(!tcp_connection_queue_item){
			LOG(ERROR) << "Could not create connection queue item for the connection";
			continue;
		}

		// Add TCP connection queue item to the queue
		m_queue.add(tcp_connection_queue_item);

		LOG(INFO) << "Connection from host '" << tcp_connection->getPeerIp() << "' added to connection queue";
	}

}

void MqttBroker::start() {
	LOG(INFO)<< "Starting MQTT Broker...";

	// Initialize TCP connection handlers (workers)
	this->startTcpConnectionHandlers();

	// Initialize TCP connection acceptor
	this->startTcpConnectionAcceptor();

	// Start accepting incomming connections
	this->startAcceptingConnections();
}

MqttBroker::~MqttBroker() {
	// Delete TCP connection acceptor
	delete m_tcp_connection_acceptor;
}

