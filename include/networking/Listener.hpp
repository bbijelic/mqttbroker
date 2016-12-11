/*
 * Listener.hpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#ifndef NETWORKING_LISTENER_HPP_
#define NETWORKING_LISTENER_HPP_

#include <string>

#include "Thread.hpp"
#include "ConnectionQueue.hpp"
#include "Connection.hpp"

using namespace std;
using namespace Concurrency;

namespace Networking {

class Listener: public Thread {
protected:

	/**
	 * Port number
	 */
	int m_port;

	/**
	 * Binding address
	 */
	string m_bind;

	/**
	 * Socket descriptor
	 */
	int m_sd;

	/**
	 * Connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

public:

	/**
	 * Constructor
	 */
	Listener(int port, string bind, ConnectionQueue<Connection*>& queue) :
			m_port(port), m_bind(bind), m_sd(0), m_queue(queue) {};

	/**
	 * Thread run method
	 */
	virtual void* run() =0;

	/**
	 * Returns port
	 */
	int getPort() { return m_port; }

	/**
	 * Returns bind address
	 */
	string getBindAdress() { return m_bind; }

};

}

#endif /* NETWORKING_LISTENER_HPP_ */
