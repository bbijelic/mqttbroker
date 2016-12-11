/*
 * ListenerManager.hpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#ifndef NETWORKING_LISTENERMANAGER_HPP_
#define NETWORKING_LISTENERMANAGER_HPP_

#include "TcpListenerManager.hpp"
#include "SslListenerManager.hpp"
#include "ServerConfiguration.hpp"
#include "ConnectionQueue.hpp"

using namespace Networking;
using namespace Networking::TCP;
using namespace Networking::Security;
using namespace Configuration;

namespace Networking {

/**
 * Listener manager
 */
class ListenerManager {
private:

	/**
	 * Connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

	/**
	 * Server configuration
	 */
	ServerConfiguration* m_config;

	/**
	 * TCP listener manager
	 */
	TcpListenerManager* m_tcp_listener_mngr;

	/**
	 * SSL listener manager
	 */
	SslListenerManager* m_ssl_listener_mngr;

public:

	/**
	 * Constructor
	 */
	ListenerManager(ConnectionQueue<Connection*>& queue,
			ServerConfiguration* config);

	/**
	 * Destructor
	 */
	~ListenerManager();

	/**
	 * Returns TCP listener manager ptr
	 */
	TcpListenerManager* getTcpListenerManager();

	/**
	 * Returns SSL listener manager ptr
	 */
	SslListenerManager* getSslListenerManager();

	/**
	 * Starts TCP and SSL listeners
	 */
	void startListeners();

	/**
	 * Stpšs TCP and SSL listeners
	 */
	void stopListeners();
};

}

#endif /* NETWORKING_LISTENERMANAGER_HPP_ */
