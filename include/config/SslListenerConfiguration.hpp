/*
 * SslListenerConfiguration.hpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#ifndef CONFIG_SSLLISTENERCONFIGURATION_HPP_
#define CONFIG_SSLLISTENERCONFIGURATION_HPP_

#include <string>

using namespace std;

namespace Configuration {

class SslListenerConfiguration {
private:

	/**
	 * Port number
	 */
	int m_port;

	/**
	 * Bind address
	 */
	string m_bind_address;

	/**
	 * CA certificate file
	 */
	string m_ca_certfile;

	/**
	 * Server certificate file
	 */
	string m_server_certfile;

	/**
	 * Server key file
	 */
	string m_server_keyfile;

	/**
	 * Verify peer
	 */
	bool m_verify_peer;

public:

	/**
	 * Constructor
	 */
	SslListenerConfiguration(int port, string bind_address, string ca_certfile,
			string server_certfile, string server_keyfile, bool verify_peer) :
			m_port(port), m_bind_address(bind_address), m_ca_certfile(ca_certfile),
			m_server_certfile(server_certfile), m_server_keyfile(server_keyfile),
			m_verify_peer(verify_peer) {};

	/**
	 * Destructor
	 */
	~SslListenerConfiguration();

	/**
	 * Port getter;
	 */
	int getPort();

	/**
	 *
	 */
	string getBindAddress();

	/**
	 * Returns CA certificate filepath
	 */
	string getCaCertFile();

	/**
	 * Returns Server certificate filepath
	 */
	string getServerCertFile();

	/**
	 * Returns Server key filepath
	 */
	string getServerKeyFile();

	/**
	 * Returns verify peer flag
	 */
	bool getVerifyPeer();

};

}

#endif /* CONFIG_SSLLISTENERCONFIGURATION_HPP_ */
