/*
 * ServerConfiguration.hpp
 *
 *  Created on: Dec 5, 2016
 *      Author: dev
 */

#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_

#include <vector>
#include <string>

#include <libconfig.h++>

#include "TcpListenerConfiguration.hpp"
#include "SslListenerConfiguration.hpp"
#include "WorkerPoolConfiguration.hpp"

using namespace std;
using namespace libconfig;

namespace Configuration {

// Node name
#define CFG_NODE_NAME "node_name"

// TCP listener
#define CFG_TCP_LISTENERS "tcp_listeners"
#define CFG_TCP_LISTENER_PORT "port"
#define CFG_TCP_LISTENER_BIND "bind"

// SSL listener
#define CFG_SSL_LISTENERS "ssl_listeners"
#define CFG_SSL_LISTENER_PORT "port"
#define CFG_SSL_LISTENER_BIND "bind"
#define CFG_SSL_LISTENER_CA_CERT_FILE "ca_cert_file"
#define CFG_SSL_LISTENER_SERVER_CERT_FILE "server_cert_file"
#define CFG_SSL_LISTENER_SERVER_KEY_FILE "server_key_file"
#define CFG_SSL_LISTENER_VERIFY_PEER "verify_peer"

// IO Worker pool
#define CFG_IO_WORKER_POOL "io_worker_pool"
#define CFG_IO_WORKER_POOL_MIN_SIZE "min_size"
#define CFG_IO_WORKER_POOL_MAX_SIZE "max_size"


/**
 * Server configuration
 */
class ServerConfiguration {
private:

	/**
	 * Broker node name
	 */
	string m_node_name;

	/**
	 * TCP Listener configuration vector
	 */
	vector<TcpListenerConfiguration*>* m_tcp_listeners;

	/**
	 * SSL Listener configuration vector
	 */
	vector<SslListenerConfiguration*>* m_ssl_listeners;

	/**
	 * Worker thread pool configuration
	 */
	WorkerPoolConfiguration* m_worker_pool;

public:

	/**
	 * Constructor
	 */
	ServerConfiguration();

	/**
	 * Destructor
	 */
	~ServerConfiguration();

	/**
	 * Returns the node name
	 */
	string getNodeName();

	/**
	 * Returns vector of TCP listeners
	 */
	vector<TcpListenerConfiguration*>* getTcpListeners();

	/**
	 * Returns vector of SSL listeners
	 */
	vector<SslListenerConfiguration*>* getSslListeners();

	/**
	 * Returns worker pool configuration
	 */
	WorkerPoolConfiguration* getWorkerPoolConfig();

	/**
	 * Parses configuration
	 */
	void parseConfiguration(string config_filepath);

};

}


#endif /* CONFIGURATION_HPP_ */
