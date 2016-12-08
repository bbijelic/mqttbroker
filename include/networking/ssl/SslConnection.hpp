/*
 * SslConnection.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef SSL_TCPCONNECTION_HPP_
#define SSL_TCPCONNECTION_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

#include <openssl/ssl.h>

#include "Connection.hpp"

using namespace std;

namespace Networking {
namespace Security {

/**
 * SSL Connection
 */
class SslConnection : public Connection {
private:

	SSL* m_ssl;

public:

	/**
	 * Constructor
	 */
	SslConnection(SSL* ssl, int socketd, struct sockaddr_in* address);

	/**
	 * Destructor
	 */
	~SslConnection();

	/**
	 * Sends via socket
	 */
	ssize_t send(const char* buffer, size_t length);

	/**
	 * Receive from the socket
	 */
	ssize_t receive(char* buffer, size_t length);

};

}
}

#endif /* SSL_TCPCONNECTION_HPP_ */
