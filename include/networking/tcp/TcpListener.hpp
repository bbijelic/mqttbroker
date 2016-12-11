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
#include "Listener.hpp"

#include <string>

using namespace std;
using namespace Concurrency;

namespace Networking {
namespace TCP {

/**
 * Listens for incomming connections
 */
class TcpListener: public Listener {
private:

	/**
	 * Accepts connection
	 */
	TcpConnection* acceptConnections();

public:

	/**
	 * Constructor
	 */
	TcpListener(int port, string bind, ConnectionQueue<Connection*>& queue) :
			Listener(port, bind, queue) {};

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
