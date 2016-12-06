/*
 * ServerConfiguration.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#include "ServerConfiguration.hpp"
#include "ConfigurationException.hpp"
#include "SslListenerConfiguration.hpp"
#include "TcpListenerConfiguration.hpp"
#include "WorkerPoolConfiguration.hpp"

#include <string>
#include <vector>
#include <libconfig.h++>
#include "easylogging++.hpp"

using namespace std;
using namespace libconfig;
using namespace Configuration;

ServerConfiguration::ServerConfiguration() {
	m_tcp_listeners = new vector<TcpListenerConfiguration*>();
	m_ssl_listeners = new vector<SslListenerConfiguration*>();

	m_worker_pool = NULL;
}

ServerConfiguration::~ServerConfiguration() {

	// Clean up TCP listeners
	LOG(DEBUG)<< "TCP Listeners ready to be destroyed: " << m_tcp_listeners->size();
	vector<TcpListenerConfiguration*>::iterator it;
	for (it = m_tcp_listeners->begin(); it < m_tcp_listeners->end(); it++) {
		delete (*it);
	}
	m_tcp_listeners->clear();
	delete m_tcp_listeners;

	// Clean up SSL listeners
	LOG(DEBUG) << "SSL Listeners ready to be destroyed: " << m_ssl_listeners->size();
	vector<SslListenerConfiguration*>::iterator ssl_it;
	for (ssl_it = m_ssl_listeners->begin(); ssl_it < m_ssl_listeners->end();
			ssl_it++) {
		delete (*ssl_it);
	}
	m_ssl_listeners->clear();
	delete m_ssl_listeners;

	// Clean up worker pool configuration
	delete m_worker_pool;
}

string ServerConfiguration::getNodeName() {
	return m_node_name;
}

vector<SslListenerConfiguration*>* ServerConfiguration::getSslListeners() {
	return m_ssl_listeners;
}

vector<TcpListenerConfiguration*>* ServerConfiguration::getTcpListeners() {
	return m_tcp_listeners;
}

WorkerPoolConfiguration* ServerConfiguration::getWorkerPoolConfig() {
	return m_worker_pool;
}

void ServerConfiguration::parseConfiguration(string config_filepath) {

	// Config instance
	Config cfg;

	try {

		// Read the server config file
		cfg.readFile(config_filepath.c_str());

	} catch (const FileIOException &fioex) {
		throw ConfigurationException(
				"Unable to read broker configuration file");

	} catch (const ParseException &pex) {
		throw ConfigurationException(
				"Unable to parse broker configuration file");
	}

	// Get config root
	const Setting& root = cfg.getRoot();

	// Get node name
	if (root.exists(CFG_NODE_NAME)) {
		// Get the broker name from the configuration file
		root.lookupValue(CFG_NODE_NAME, m_node_name);
	} else {
		throw ConfigurationException(
				"Configuration node 'CFG_BROKER_NODE_NAME' not defined");
	}

	// Get plain TCP connection listener configs
	if (root.exists(CFG_TCP_LISTENERS)) {
		// Get tcp listeners
		Setting& tcp_listeners = root.lookup(CFG_TCP_LISTENERS);

		for (int i = 0; i < tcp_listeners.getLength(); i++) {
			// Get setting instance
			Setting& tcp_listener = tcp_listeners[i];

			int port;
			string bind;

			// Check if port is defined
			if (tcp_listener.exists(CFG_TCP_LISTENER_PORT)) {
				tcp_listener.lookupValue(CFG_TCP_LISTENER_PORT, port);
			}

			// Check if bind is defined
			if (tcp_listener.exists(CFG_TCP_LISTENER_BIND)) {
				tcp_listener.lookupValue(CFG_TCP_LISTENER_BIND, bind);
			}

			// TCP listener configuration instance
			TcpListenerConfiguration* tcp_listener_config =
					new TcpListenerConfiguration(port, bind);

			// Add configuration to the vector
			m_tcp_listeners->push_back(tcp_listener_config);

		}

	} else {
		throw ConfigurationException(
				"Configuration node 'tcp_listeners' not defined");
	}

	// Get SSL connection listener configs
	if (root.exists(CFG_SSL_LISTENERS)) {
		// Get tcp listeners
		Setting& ssl_listeners = root.lookup(CFG_SSL_LISTENERS);

		for (int i = 0; i < ssl_listeners.getLength(); i++) {
			// Get setting instance
			Setting& ssl_listener = ssl_listeners[i];

			int port;
			string bind, ca_cert_file, server_cert_file, server_key_file;
			bool verify_peer;

			// Check if port is defined
			if (ssl_listener.exists(CFG_SSL_LISTENER_PORT)) {
				ssl_listener.lookupValue(CFG_SSL_LISTENER_PORT, port);
			}

			// Check if bind is defined
			if (ssl_listener.exists(CFG_SSL_LISTENER_BIND)) {
				ssl_listener.lookupValue(CFG_SSL_LISTENER_BIND, bind);
			}

			// Check if bind is defined
			if (ssl_listener.exists(CFG_SSL_LISTENER_CA_CERT_FILE)) {
				ssl_listener.lookupValue(CFG_SSL_LISTENER_CA_CERT_FILE,
						ca_cert_file);
			}

			// Check if bind is defined
			if (ssl_listener.exists(CFG_SSL_LISTENER_SERVER_CERT_FILE)) {
				ssl_listener.lookupValue(CFG_SSL_LISTENER_SERVER_CERT_FILE,
						server_cert_file);
			}

			// Check if bind is defined
			if (ssl_listener.exists(CFG_SSL_LISTENER_SERVER_KEY_FILE)) {
				ssl_listener.lookupValue(CFG_SSL_LISTENER_SERVER_KEY_FILE,
						server_key_file);
			}

			// Check if bind is defined
			if (ssl_listener.exists(CFG_SSL_LISTENER_VERIFY_PEER)) {
				ssl_listener.lookupValue(CFG_SSL_LISTENER_VERIFY_PEER,
						verify_peer);
			}

			// SSL listener configuration instance
			SslListenerConfiguration* ssl_listener_config =
					new SslListenerConfiguration(port, bind, ca_cert_file,
							server_cert_file, server_key_file, verify_peer);

			// Add configuration to the vector
			m_ssl_listeners->push_back(ssl_listener_config);

		}

	} else {
		throw ConfigurationException(
				"Configuration node 'ssl_listeners' not defined");
	}

	// Get node name
	if (root.exists(CFG_IO_WORKER_POOL)) {

		// Get tcp listeners
		Setting& io_worker_pool = root.lookup(CFG_IO_WORKER_POOL);

		int min_size = 1, max_size = 1;

		// Check if min size is defined
		if (io_worker_pool.exists(CFG_IO_WORKER_POOL_MIN_SIZE)) {
			io_worker_pool.lookupValue(CFG_IO_WORKER_POOL_MIN_SIZE, min_size);
		}

		// Check if min size is defined
		if (io_worker_pool.exists(CFG_IO_WORKER_POOL_MAX_SIZE)) {
			io_worker_pool.lookupValue(CFG_IO_WORKER_POOL_MAX_SIZE, max_size);
		}

		// Initialize worker pool config
		m_worker_pool = new WorkerPoolConfiguration(min_size, max_size);

	} else {
		throw ConfigurationException(
				"Configuration node 'io_worker_pool' not defined");
	}

}

