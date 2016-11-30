/*
 * TcpConnectionHandler.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef TCP_TCPCONNECTIONHANDLER_HPP_
#define TCP_TCPCONNECTIONHANDLER_HPP_

#include "TcpConnectionQueue.hpp"
#include "TcpConnection.hpp"
#include "Thread.hpp"

using namespace Concurrency;

namespace TCP {

class TcpConnectionHandler : public Thread {
private:

	/**
	 * TCP connection queue
	 */
	TcpConnectionQueue<TcpConnection*>& m_queue;

	/**
	 * Epoll file descriptor
	 */
	int m_epoll_fd;

	/**
	 * Makes socket non-blocking so it can be used with epoll in edge trigger mode
	 */
	bool makeSocketNonBlocking(int socketd);

	/**
	 * Registers socket for the events on the epoll
	 */
	bool registerSocketToEpoll(TcpConnection *connection);

public:

	/**
	 * Constructor
	 */
	TcpConnectionHandler(TcpConnectionQueue<TcpConnection*>& queue, int epoll_fd);

	/**
	 * Run
	 */
	void* run();

};

}

#endif /* TCP_TCPCONNECTIONHANDLER_HPP_ */
