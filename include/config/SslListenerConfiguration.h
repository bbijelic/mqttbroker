#ifndef CONFIG_SSLLISTENERCONFIGURATION_H
#define CONFIG_SSLLISTENERCONFIGURATION_H

#include <string>

namespace Broker {
namespace Config {

class SslListenerConfiguration {
private:

	/**
	 * Port number
	 */
	int m_port;

	/**
	 * Bind address
	 */
	std::string m_bind_address;

	/**
	 * CA certificate file
	 */
	std::string m_ca_certfile;

	/**
	 * Server certificate file
	 */
	std::string m_server_certfile;

	/**
	 * Server key file
	 */
	std::string m_server_keyfile;

	/**
	 * Verify peer
	 */
	bool m_verify_peer;

public:

	/**
	 * Constructor
	 */
	SslListenerConfiguration(int port, std::string bind_address, std::string ca_certfile,
			std::string server_certfile, std::string server_keyfile, bool verify_peer) :
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
	std::string getBindAddress();

	/**
	 * Returns CA certificate filepath
	 */
	std::string getCaCertFile();

	/**
	 * Returns Server certificate filepath
	 */
	std::string getServerCertFile();

	/**
	 * Returns Server key filepath
	 */
	std::string getServerKeyFile();

	/**
	 * Returns verify peer flag
	 */
	bool getVerifyPeer();

};

}}

#endif