#include "config/ServerConfiguration.h"
#include "config/ConfigurationException.h"
#include "config/SslListenerConfiguration.h"
#include "config/TcpListenerConfiguration.h"
#include "config/WorkerPoolConfiguration.h"
#include "logging/easylogging++.h"

#include <string>
#include <vector>
#include <libconfig.h++>

Broker::Config::ServerConfiguration::ServerConfiguration() {
	m_tcp_listeners = new std::vector<Config::TcpListenerConfiguration*>();
	m_ssl_listeners = new std::vector<Config::SslListenerConfiguration*>();

	m_worker_pool = NULL;
}

Broker::Config::ServerConfiguration::~ServerConfiguration() {

	// Clean up TCP listeners
	LOG(DEBUG)<< "TCP Listeners ready to be destroyed: " << m_tcp_listeners->size();
	std::vector<Broker::Config::TcpListenerConfiguration*>::iterator it;
	for (it = m_tcp_listeners->begin(); it < m_tcp_listeners->end(); it++) {
		delete (*it);
	}
	m_tcp_listeners->clear();
	delete m_tcp_listeners;

	// Clean up SSL listeners
	LOG(DEBUG) << "SSL Listeners ready to be destroyed: " << m_ssl_listeners->size();
	std::vector<Broker::Config::SslListenerConfiguration*>::iterator ssl_it;
	for (ssl_it = m_ssl_listeners->begin(); ssl_it < m_ssl_listeners->end();
			ssl_it++) {
		delete (*ssl_it);
	}
	m_ssl_listeners->clear();
	delete m_ssl_listeners;

	// Clean up worker pool configuration
	delete m_worker_pool;
}

std::string Broker::Config::ServerConfiguration::getNodeName() {
	return m_node_name;
}

std::vector<Broker::Config::SslListenerConfiguration*>* Broker::Config::ServerConfiguration::getSslListeners() {
	return m_ssl_listeners;
}

std::vector<Broker::Config::TcpListenerConfiguration*>* Broker::Config::ServerConfiguration::getTcpListeners() {
	return m_tcp_listeners;
}

Broker::Config::WorkerPoolConfiguration* Broker::Config::ServerConfiguration::getWorkerPoolConfig() {
	return m_worker_pool;
}

void Broker::Config::ServerConfiguration::parseConfiguration(std::string config_filepath) {

	// Config instance
	libconfig::Config cfg;

	try {

		// Read the server config file
		cfg.readFile(config_filepath.c_str());

	} catch (const libconfig::FileIOException &fioex) {
		throw Broker::Config::ConfigurationException(
				"Unable to read broker configuration file");

	} catch (const libconfig::ParseException &pex) {
		throw Broker::Config::ConfigurationException(
				"Unable to parse broker configuration file");
	}

	// Get config root
	const libconfig::Setting& root = cfg.getRoot();

	// Get node name
	if (root.exists(CFG_NODE_NAME)) {
		// Get the broker name from the configuration file
		root.lookupValue(CFG_NODE_NAME, m_node_name);
	} else {
		throw Broker::Config::ConfigurationException(
				"Configuration node 'CFG_BROKER_NODE_NAME' not defined");
	}

	// Get plain TCP connection listener configs
	if (root.exists(CFG_TCP_LISTENERS)) {
		// Get tcp listeners
		libconfig::Setting& tcp_listeners = root.lookup(CFG_TCP_LISTENERS);

		for (int i = 0; i < tcp_listeners.getLength(); i++) {
			// Get setting instance
			libconfig::Setting& tcp_listener = tcp_listeners[i];

			int port;
			std::string bind;

			// Check if port is defined
			if (tcp_listener.exists(CFG_TCP_LISTENER_PORT)) {
				tcp_listener.lookupValue(CFG_TCP_LISTENER_PORT, port);
			}

			// Check if bind is defined
			if (tcp_listener.exists(CFG_TCP_LISTENER_BIND)) {
				tcp_listener.lookupValue(CFG_TCP_LISTENER_BIND, bind);
			}

			// TCP listener configuration instance
			Broker::Config::TcpListenerConfiguration* tcp_listener_config =
					new Config::TcpListenerConfiguration(port, bind);

			// Add configuration to the vector
			m_tcp_listeners->push_back(tcp_listener_config);

		}

	} else {
		throw Broker::Config::ConfigurationException(
				"Configuration node 'tcp_listeners' not defined");
	}

	// Get SSL connection listener configs
	if (root.exists(CFG_SSL_LISTENERS)) {
		// Get tcp listeners
		libconfig::Setting& ssl_listeners = root.lookup(CFG_SSL_LISTENERS);

		for (int i = 0; i < ssl_listeners.getLength(); i++) {
			// Get setting instance
			libconfig::Setting& ssl_listener = ssl_listeners[i];

			int port;
			std::string bind, ca_cert_file, server_cert_file, server_key_file;
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
			Broker::Config::SslListenerConfiguration* ssl_listener_config =
					new Config::SslListenerConfiguration(port, bind, ca_cert_file,
							server_cert_file, server_key_file, verify_peer);

			// Add configuration to the vector
			m_ssl_listeners->push_back(ssl_listener_config);

		}

	} else {
		throw Broker::Config::ConfigurationException(
				"Configuration node 'ssl_listeners' not defined");
	}

	// Get node name
	if (root.exists(CFG_IO_WORKER_POOL)) {

		// Get tcp listeners
		libconfig::Setting& io_worker_pool = root.lookup(CFG_IO_WORKER_POOL);

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
		m_worker_pool = new Broker::Config::WorkerPoolConfiguration(min_size, max_size);

	} else {
		throw Broker::Config::ConfigurationException(
				"Configuration node 'io_worker_pool' not defined");
	}

}

