/*
 * TcpListenerManager.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#include "TcpListenerManager.hpp"

#include "easylogging++.hpp"

using namespace std;
using namespace Networking::TCP;

TcpListenerManager::~TcpListenerManager() {
	LOG(DEBUG)<< "Destroying TCP listener manager";
}

void TcpListenerManager::startListeners() {
	LOG(DEBUG)<< "Starting TCP listeners";
	vector<TcpListenerConfiguration*>::iterator it;
	for (it = m_listeners_config->begin(); it < m_listeners_config->end();
			it++) {

		LOG(DEBUG) << "Starting TCP listener on port " << (*it)->getPort() << " bound to " << (*it)->getBindAddress();

		// New instance of tcp listener
		TcpListener* tcp_listener = new TcpListener((*it)->getPort(),
				(*it)->getBindAddress(), m_queue);

		// Start the listener
		tcp_listener->start();

		// Add listener to the vector so it can be stopped later
		m_listeners.push_back(tcp_listener);
	}
}

void TcpListenerManager::stopListeners() {
	LOG(DEBUG)<< "Stopping TCP listeners";

	while (!m_listeners.empty()) {
		// Get listener
		TcpListener* listener = m_listeners.back();

		LOG(DEBUG) << "Stopping TCP listener on port " << listener->getPort() << " bound to " << listener->getBindAdress();

		// Stop listener
		listener->gracefulStop();
		// Remove listener from the vecotr
		m_listeners.pop_back();
		// Destroy listener
		delete listener;
	}
}

