/*
 * SslListener.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: dev
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>

#include <easylogging++.hpp>
#include <SslListener.hpp>

using namespace std;
using namespace Networking::Security;

void SslListener::initializeSSL() {
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
}

void SslListener::destroySSL() {
	ERR_free_strings();
	EVP_cleanup();
}

void SslListener::shutdownSSL(SSL* ssl) {
	SSL_shutdown(ssl);
	SSL_free(ssl);
}

SslConnection* SslListener::acceptConnections() {

	// Peer address
	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	memset(&address, 0, len);

	// Accept new connection
	int con_sd = ::accept(m_sd, (struct sockaddr*) &address, &len);

	LOG(INFO)<< "SSL listener accepted connection on socket " << con_sd;

	if (con_sd < 0) {
		LOG(ERROR)<< "SSL listener accept() failed: " << errno;
		return NULL;
	}

	// New instance of SSL by using configured context
	SSL* ssl = SSL_new(m_ssl_ctx);

	// Associate new connection socket with SSL
	SSL_set_fd(ssl, con_sd);

	// Here is the SSL Accept portion.
	// Now all reads and writes must use SSL
	int ssl_err = SSL_accept(ssl);
	if (ssl_err <= 0) {
		LOG(ERROR)<< "SSL listener handshake failed on the socket " << con_sd;

		// Shutdown SSL
		shutdownSSL(ssl);

		return NULL;
	}

	// Create new connection instance and return ptr
	return new SslConnection(ssl, con_sd, &address);
}

void* SslListener::run() {

	// Initialize SSL
	this->initializeSSL();

	// Open socket
	m_sd = socket(PF_INET, SOCK_STREAM, 0);

	// Terminate listener if error occurs
	if (m_sd < 0)
		return NULL;

	struct sockaddr_in address;

	// Set 0 to address
	memset(&address, 0, sizeof(address));

	address.sin_family = PF_INET;
	address.sin_port = htons(m_port);
	address.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	setsockopt(m_sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	// Bind to the interface
	int result = bind(m_sd, (struct sockaddr*) &address, sizeof(address));

	if (result != 0) {
		LOG(ERROR)<< "SSL listener bind() failed: " << errno;
		return NULL;
	}

	// Start listening
	result = listen(m_sd, 5);
	if (result != 0) {
		LOG(ERROR)<< "SSL listener listen() failed";
		return NULL;
	}

	LOG(INFO)<< "SSL listener is listening on port " << m_port;

	// Initialize SSL context
	m_ssl_ctx = SSL_CTX_new(TLSv1_2_server_method());
	// Set SSL context options
	SSL_CTX_set_options(m_ssl_ctx, SSL_OP_SINGLE_DH_USE);

	// Load server certificate and private key
	SSL_CTX_use_certificate_file(m_ssl_ctx, m_server_cert_file.c_str(),
	SSL_FILETYPE_PEM);
	SSL_CTX_use_PrivateKey_file(m_ssl_ctx, m_server_key_file.c_str(),
	SSL_FILETYPE_PEM);

	if (m_verify_peer) {
		STACK_OF(X509_NAME) *list;
		list = SSL_load_client_CA_file(m_ca_cert_file.c_str());
		SSL_CTX_load_verify_locations(m_ssl_ctx, m_ca_cert_file.c_str(), NULL);
		SSL_CTX_set_client_CA_list(m_ssl_ctx, list);
		SSL_CTX_set_verify(m_ssl_ctx, SSL_VERIFY_PEER, NULL);
	}

	// Loop indefinitely and accept connections
	while (true) {

		LOG(INFO)<< "SSL listener waiting for incomming connections";

		// Blocks until connection is returned
		SslConnection* connection = acceptConnections();

		if(!connection) {
			LOG(ERROR) << "SSL listener could not accept connection";
			continue;
		}

		// Adding connection to the queue
		m_queue.add(connection);

		LOG(INFO) << "SSL listener added connection from " << connection->getPeerIp() << " to the IO Queue";
	}
}

SslListener::~SslListener() {

	// Destroy SSL
	destroySSL();

	if (m_sd > 0) {
		LOG(INFO)<< "SSL listener closing socket " << m_sd;
		close(m_sd);
	}
}

