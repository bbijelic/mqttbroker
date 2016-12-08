/*
 * ConnectionHandler.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef NETWORKING_CONNECTIONHANDLER_HPP_
#define NETWORKING_CONNECTIONHANDLER_HPP_

#include "ConnectionQueue.hpp"
#include "Connection.hpp"
#include "Thread.hpp"

using namespace Concurrency;

namespace Networking {

class ConnectionHandler : public Thread {
private:

	/**
	 * TCP connection queue
	 */
	ConnectionQueue<Connection*>& m_queue;

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
	bool registerSocketToEpoll(Connection *connection);

public:

	/**
	 * Constructor
	 */
	ConnectionHandler(ConnectionQueue<Connection*>& queue, int epoll_fd);

	/**
	 * Run
	 */
	void* run();

};

}

#endif /* NETWORKING_CONNECTIONHANDLER_HPP_ */
