/*
 * SslListener.hpp
 *
 *  Created on: Dec 8, 2016
 *      Author: dev
 */

#ifndef SSL_SSLLISTENER_HPP_
#define SSL_SSLLISTENER_HPP_

#include "ConnectionQueue.hpp"
#include "Thread.hpp"
#include "SslConnection.hpp"
#include "Connection.hpp"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

using namespace Networking;
using namespace Concurrency;

namespace Networking {
namespace Security {

/**
 * SSL listener
 */
class SslListener: public Thread {
private:

	/**
	 * TCP connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

	/**
	 * Port
	 */
	int m_port;

	/**
	 * CA certificate file
	 */
	string m_ca_cert_file;

	/**
	 * Server certificate file
	 */
	string m_server_cert_file;

	/**
	 * Server key file
	 */
	string m_server_key_file;

	/**
	 * Verify peer
	 */
	bool m_verify_peer;

	/**
	 * Socket descriptor
	 */
	int m_sd;

	/**
	 * SSL context
	 */
	SSL_CTX* m_ssl_ctx;

	/**
	 * Accepts connection
	 */
	SslConnection* acceptConnections();

	/**
	 * Initialize SSL
	 */
	void initializeSSL();

	/**
	 * Destroy ssl
	 */
	void destroySSL();

	/**
	 * Shutdown SSL
	 */
	void shutdownSSL(SSL* ssl);

public:

	/**
	 * Constructor
	 */
	SslListener(ConnectionQueue<Connection*>& queue, int port,
			string ca_cert_file, string server_cert_file,
			string server_key_file, bool verify_peer) :
			m_queue(queue), m_port(port), m_ca_cert_file(ca_cert_file), m_server_cert_file(
					server_cert_file), m_server_key_file(server_key_file), m_verify_peer(
					verify_peer), m_sd(0), m_ssl_ctx(NULL) {
	}

	/**
	 * Destructor
	 */
	~SslListener();

	/**
	 * Run
	 */
	void* run();

};

}
}

#endif /* SSL_SSLLISTENER_HPP_ */
