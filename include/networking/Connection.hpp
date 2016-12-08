/*
 * Connection.hpp
 *
 *  Created on: Dec 8, 2016
 *      Author: dev
 */

#ifndef NETWORKING_CONNECTION_HPP_
#define NETWORKING_CONNECTION_HPP_

#include <string>

using namespace std;

namespace Networking {

/**
 * Connection interface
 */
class Connection {
protected:

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

public:
	/**
	 * Returns peer ip address
	 */
	string getPeerIp();

	/**
	 * Returns peer port
	 */
	int getPeerPort();

	/**
	 * Returns socket file descriptor
	 */
	int getSocket();

	/**
	 * Sends via socket
	 */
	virtual ssize_t send(const char* buffer, size_t length) =0;

	/**
	 * Receive from the socket
	 */
	virtual ssize_t receive(char* buffer, size_t length) =0;

	/**
	 * Closes connection
	 */
	void closeConnection();

};

}

#endif /* NETWORKING_CONNECTION_HPP_ */
