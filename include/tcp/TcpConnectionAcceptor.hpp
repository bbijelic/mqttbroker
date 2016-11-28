/*
 * TcpConnectionAcceptor.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef TCP_TCPCONNECTIONACCEPTOR_HPP_
#define TCP_TCPCONNECTIONACCEPTOR_HPP_

#include <string>
#include <netinet/in.h>

#include <TcpConnection.hpp>

namespace TCP {

/**
 * TCP Connection acceptor
 */
class TcpConnectionAcceptor {
private:

	/**
	 * ???
	 */
	int m_lsd;

	/**
	 * Port
	 */
	int m_port;

	/**
	 * Address
	 */
	string m_address;

	/**
	 * Is listening
	 */
	bool m_listening;

	/**
	 * Private constructor
	 */
	TcpConnectionAcceptor(){}

public:

	/**
	 * Constructor
	 */
	TcpConnectionAcceptor(int port, const char* address = "");

	/**
	 * Destructor
	 */
	~TcpConnectionAcceptor();

	/**
	 * Starts acceptor
	 */
	int start();

	/**
	 * Accepts connection
	 */
	TcpConnection* accept();

};

}



#endif /* TCP_TCPCONNECTIONACCEPTOR_HPP_ */
