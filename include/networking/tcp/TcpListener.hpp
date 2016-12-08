/*
 * TcpListener.hpp
 *
 *  Created on: Nov 29, 2016
 *      Author: dev
 */

#ifndef TCPLISTENER_HPP_
#define TCPLISTENER_HPP_

#include "TcpConnection.hpp"
#include "ConnectionQueue.hpp"
#include "Connection.hpp"
#include "Thread.hpp"

using namespace Concurrency;

namespace Networking {
namespace TCP {

/**
 * Listens for incomming connections
 */
class TcpListener: public Thread {
private:

	/**
	 * Port
	 */
	int m_port;

	/**
	 * Connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

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
	TcpListener(int port, ConnectionQueue<Connection*>& queue) :
			m_port(port), m_queue(queue), m_sd(0) {
	}
	;

	/**
	 * Destructor
	 */
	~TcpListener();

	/**
	 * Run
	 */
	void* run();

};

}
}

#endif /* TCPLISTENER_HPP_ */
