#ifndef NET_TCP_TCPLISTENERMANAGER_H
#define NET_TCP_TCPLISTENERMANAGER_H

#include "../Connector.h"

namespace Broker {
namespace Net {
namespace TCP {

/**
 * TCP listener manager
 */
class TcpListenerManager : public Connector {
private:

    

public:

    /**
	 * Constructor
	 */
	TcpListenerManager(int port, string bind, Epoll*) :
			Connector(port, bind, queue) {};
    
    /**
	 * Destructor
	 */
	~TcpListenerManager();
    
};

}}}

#endif