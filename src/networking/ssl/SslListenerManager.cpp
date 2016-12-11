/*
 * SslListenerManager.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#include "SslListenerManager.hpp"

#include "easylogging++.hpp"

using namespace std;
using namespace Networking::Security;

SslListenerManager::~SslListenerManager() {
	LOG(DEBUG)<< "Destroying SSL listener manager";
}

void SslListenerManager::startListeners() {
	LOG(DEBUG)<< "Starting SSL listeners";
	vector<SslListenerConfiguration*>::iterator it;
	for (it = m_listeners_config->begin(); it < m_listeners_config->end();
			it++) {

		LOG(DEBUG) << "Starting SSL listener on port " << (*it)->getPort() << " bound to " << (*it)->getBindAddress();

		// New instance of SSL listener
		SslListener* Ssl_listener = new SslListener((*it)->getPort(),
				(*it)->getBindAddress(), m_queue, (*it)->getCaCertFile(), (*it)->getServerCertFile(), (*it)->getServerKeyFile(), (*it)->getVerifyPeer());

		// Start the listener
		Ssl_listener->start();

		// Add listener to the vector so it can be stopped later
		m_listeners.push_back(Ssl_listener);
	}
}

void SslListenerManager::stopListeners() {
	LOG(DEBUG)<< "Stopping SSL listeners";

	while (!m_listeners.empty()) {
		// Get listener
		SslListener* listener = m_listeners.back();

		LOG(DEBUG) << "Stopping SSL listener on port " << listener->getPort() << " bound to " << listener->getBindAdress();

		// Stop listener
		listener->gracefulStop();
		// Remove listener from the vecotr
		m_listeners.pop_back();
		// Destroy listener
		delete listener;
	}
}

