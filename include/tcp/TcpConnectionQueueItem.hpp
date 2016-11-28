#ifndef TCP_TCPCONNECTIONQUEUEITEM_HPP_
#define TCP_TCPCONNECTIONQUEUEITEM_HPP_

#include "TcpConnection.hpp"

namespace TCP {

/**
 * TCP connection queue item
 */
class TcpConnectionQueueItem {
private:

	/**
	 * TCP Connection
	 */
	TcpConnection* m_tcp_connection;

public:

	/**
	 * Constructor
	 */
	TcpConnectionQueueItem(TcpConnection* connection);

	/**
	 * Destructor
	 */
	~TcpConnectionQueueItem();

	/**
	 * Returns TCP connection
	 */
	TcpConnection* getConnection();

};

}

#endif
