/*
 * SslListenerManager.hpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#ifndef NETWORKING_SSL_SSLLISTENERMANAGER_HPP_
#define NETWORKING_SSL_SSLLISTENERMANAGER_HPP_

#include "SslListener.hpp"
#include "SslListenerConfiguration.hpp"
#include "ConnectionQueue.hpp"

#include <vector>

using namespace std;
using namespace Networking;
using namespace Configuration;

namespace Networking {
namespace Security {

/**
 * SSL Listener manager
 */
class SslListenerManager {
private:

	/**
	 * Connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

	/**
	 * Listeners
	 */
	vector<SslListener*> m_listeners;

	/**
	 * SSL listener configuration
	 */
	vector<SslListenerConfiguration*>* m_listeners_config;

public:

	/**
	 * Constructor
	 */
	SslListenerManager(ConnectionQueue<Connection*>& queue,
			vector<SslListenerConfiguration*>* listeners_config) :
			m_queue(queue), m_listeners_config(listeners_config) {
	};

	/**
	 * Destructor
	 */
	~SslListenerManager();

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
