/*
 * TcpConnection.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef TCP_TCPCONNECTION_HPP_
#define TCP_TCPCONNECTION_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

#include "Connection.hpp"

using namespace std;

namespace Networking {
namespace TCP {

/**
 * Tcp Connection
 */
class TcpConnection : public Connection {
public:

	/**
	 * Constructor
	 */
	TcpConnection(int socketd, struct sockaddr_in* address);

	/**
	 * Constructor
	 */
	TcpConnection(const TcpConnection& tcpConnection);

	/**
	 * Destructor
	 */
	~TcpConnection();

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

#endif /* TCP_TCPCONNECTION_HPP_ */
