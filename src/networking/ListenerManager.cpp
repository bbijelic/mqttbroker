/*
 * ListenerManager.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: dev
 */

#include "ListenerManager.hpp"
#include "SslListenerManager.hpp"
#include "TcpListenerManager.hpp"

using namespace Networking;
using namespace Networking::TCP;
using namespace Networking::Security;
using namespace Configuration;

ListenerManager::ListenerManager(ConnectionQueue<Connection*>& queue,
		ServerConfiguration* config) :
		m_queue(queue), m_config(config), m_tcp_listener_mngr(NULL), m_ssl_listener_mngr(
		NULL) {

	// Instance of TCP listener manager
	m_tcp_listener_mngr = new TcpListenerManager(m_queue,
			m_config->getTcpListeners());

	// Instance of SSL listener manager
	m_ssl_listener_mngr = new SslListenerManager(m_queue,
			m_config->getSslListeners());

}

ListenerManager::~ListenerManager(){
	delete m_tcp_listener_mngr;
	delete m_ssl_listener_mngr;
}

SslListenerManager* ListenerManager::getSslListenerManager() {
	return m_ssl_listener_mngr;
}

TcpListenerManager* ListenerManager::getTcpListenerManager() {
	return m_tcp_listener_mngr;
}

void ListenerManager::startListeners() {
	// Start TCP listeners
	m_tcp_listener_mngr->startListeners();
	// Start SSL listeners
	m_ssl_listener_mngr->startListeners();
}

void ListenerManager::stopListeners() {
	// Stop TCP listeners
	m_tcp_listener_mngr->stopListeners();
	// Stop SSL listeners
	m_ssl_listener_mngr->stopListeners();
}

