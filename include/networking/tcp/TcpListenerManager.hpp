/*
 * TcpListenerManager.hpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#ifndef NETWORKING_TCP_TCPLISTENERMANAGER_HPP_
#define NETWORKING_TCP_TCPLISTENERMANAGER_HPP_

#include "TcpListener.hpp"
#include "TcpListenerConfiguration.hpp"
#include "ConnectionQueue.hpp"

#include <vector>

using namespace std;
using namespace Networking;
using namespace Configuration;

namespace Networking {
namespace TCP {

/**
 * TCP Listener manager
 */
class TcpListenerManager {
private:

	/**
	 * Connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

	/**
	 * Listeners
	 */
	vector<TcpListener*> m_listeners;

	/**
	 * Tcp listener configuration
	 */
	vector<TcpListenerConfiguration*>* m_listeners_config;

public:

	/**
	 * Constructor
	 */
	TcpListenerManager(ConnectionQueue<Connection*>& queue,
			vector<TcpListenerConfiguration*>* listeners_config) :
			m_queue(queue), m_listeners_config(listeners_config) {
	};

	/**
	 * Destructor
	 */
	~TcpListenerManager();

	/**
	 * Starts listeners
	 */
	void startListeners();

	/**
	 * Stops listeners
	 */
	void stopListeners();
};

}
}

#endif /* NETWORKING_TCP_TCPLISTENERMANAGER_HPP_ */
