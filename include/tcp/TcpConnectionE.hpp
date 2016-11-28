#ifndef TCP_TCPCONNECTIONE_HPP
#define TCP_TCPCONNECTIONE_HPP

namespace TCP {

enum TcpConnectionE {

	/**
	 * Connection closed
	 */
	CONNECTION_CLOSED = 0,

	/**
	 * Connection reset
	 */
	CONNECTION_RESET = -1,

	/**
	 * Connection timed out
	 */
	CONNECTION_TIMED_OUT = -2
};

}

#endif
