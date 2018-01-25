#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>
#include <string>

#include "TcpListenerConfiguration.h"
#include "SslListenerConfiguration.h"
#include "WorkerPoolConfiguration.h"

namespace Broker {
namespace Config {

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
	std::string m_node_name;

	/**
	 * TCP Listener configuration vector
	 */
	std::vector<TcpListenerConfiguration*>* m_tcp_listeners;

	/**
	 * SSL Listener configuration vector
	 */
	std::vector<SslListenerConfiguration*>* m_ssl_listeners;

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
	std::string getNodeName();

	/**
	 * Returns vector of TCP listeners
	 */
	std::vector<TcpListenerConfiguration*>* getTcpListeners();

	/**
	 * Returns vector of SSL listeners
	 */
	std::vector<SslListenerConfiguration*>* getSslListeners();

	/**
	 * Returns worker pool configuration
	 */
	WorkerPoolConfiguration* getWorkerPoolConfig();

	/**
	 * Parses configuration
	 */
	void parseConfiguration(std::string config_filepath);

};

}}


#endif
