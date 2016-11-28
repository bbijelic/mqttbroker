/*
 * TcpConnectionHandler.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef TCP_TCPCONNECTIONHANDLER_HPP_
#define TCP_TCPCONNECTIONHANDLER_HPP_

#include "TcpConnectionQueue.hpp"
#include "TcpConnectionQueueItem.hpp"
#include "Thread.hpp"

using namespace Concurrency;

namespace TCP {

class TcpConnectionHandler : public Thread {
private:

	/**
	 * TCP connection queue
	 */
	TcpConnectionQueue<TcpConnectionQueueItem*>& m_queue;

public:

	/**
	 * Constructor
	 */
	TcpConnectionHandler(TcpConnectionQueue<TcpConnectionQueueItem*>& queue);

	/**
	 * Run
	 */
	void* run();

};

}

#endif /* TCP_TCPCONNECTIONHANDLER_HPP_ */
