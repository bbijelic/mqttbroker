#ifndef NET_LISTENERMANAGER_H
#define NET_LISTENERMANAGER_H

#include <string>

namespace Broker {
namespace Net {
    
/**
 * Connector
 */
class Connector {
private:

    // Port number
	int m_port;

	// Binding address
	std::string m_bind_address;

	// Socket descriptor
	int m_socket_descriptor;    

    
    
public:

    // Constructor
	Connector(int port, std::string bind) :
			m_port(port), m_bind_address(bind), m_socket_descriptor(0) {};
			
	// Returns port
	int getPort() { return m_port; }

	// Returns bind address
	std::string getBindAdress() { return m_bind_address; }

};

}}

#endif