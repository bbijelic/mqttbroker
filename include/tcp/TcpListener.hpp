/*
 * TcpListener.hpp
 *
 *  Created on: Nov 29, 2016
 *      Author: dev
 */

#ifndef TCPLISTENER_HPP_
#define TCPLISTENER_HPP_

#include "TcpConnection.hpp"
#include "TcpConnectionQueue.hpp"

namespace TCP {

/**
 * Listens for incomming connections
 */
class TcpListener {
private:

	/**
	 * Port
	 */
	int m_port;

	/**
	 * TCP connection queue
	 */
	TcpConnectionQueue<TcpConnection*>& m_queue;

	/**
	 * Socket descriptor
	 */
	int m_sd;

	/**
	 * Accepts connection
	 */
	TcpConnection* acceptConnections();

public:

	/**
	 * Constructor
	 */
	TcpListener(int port, TcpConnectionQueue<TcpConnection*>& queue) :
			m_port(port), m_queue(queue), m_sd(0) {
	};

	/**
	 * Destructor
	 */
	~TcpListener();

	/**
	 * Run
	 */
	void startListening();

};

}

#endif /* TCPLISTENER_HPP_ */
