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

using namespace std;

namespace TCP {

/**
 * Tcp Connection
 */
class TcpConnection {
private:

	/**
	 * Socket descriptor
	 */
	int m_sd;

	/**
	 * Peer IP address
	 */
	string m_peer_ip;

	/**
	 * Peer port
	 */
	int m_peer_port;

	/**
	 * Wait for the read event
	 */
	bool waitForReadEvent(int timeout);


public:

	/**
	 * Constructor
	 */
	TcpConnection();

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
	 * Returns peer ip address
	 */
	string getPeerIp();

	/**
	 * Returns peer port
	 */
	int getPeerPort();

	/**
	 * Sends via socket
	 */
	ssize_t send(const char* buffer, size_t length);

	/**
	 * Receive from the socket
	 */
	ssize_t receive(char* buffer, size_t length, int timeout=0);
};

}

#endif /* TCP_TCPCONNECTION_HPP_ */
