#ifndef CONFIG_TCPLISTENERCONFIGURATION_H
#define CONFIG_TCPLISTENERCONFIGURATION_H

#include <string>

namespace Broker {
namespace Config {

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
	std::string m_bind_address;

public:

	/**
	 * Constructor
	 */
	TcpListenerConfiguration(int port, std::string bind_address) :
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
	std::string getBindAddress();
};

}}

#endif