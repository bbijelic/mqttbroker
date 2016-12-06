/*
 * TcpListenerConfiguration.hpp
 *
 *  Created on: Dec 5, 2016
 *      Author: dev
 */

#ifndef CONFIG_TCPLISTENERCONFIGURATION_HPP_
#define CONFIG_TCPLISTENERCONFIGURATION_HPP_

#include <string>

using namespace std;

namespace Configuration {

/**
 * Tcp listener configuration
 */
class TcpListenerConfiguration {
private:

	/**
	 * Port number
	 */
	int m_port;

	/**
	 * Bind address
	 */
	string m_bind_address;

public:

	/**
	 * Constructor
	 */
	TcpListenerConfiguration(int port, string bind_address) :
			m_port(port), m_bind_address(bind_address) {};

	/**
	 * Destructor
	 */
	~TcpListenerConfiguration();

	/**
	 * Port getter;
	 */
	int getPort();

	/**
	 *
	 */
	string getBindAddress();
};

}

#endif /* CONFIG_TCPLISTENERCONFIGURATION_HPP_ */
