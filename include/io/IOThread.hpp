/*
 * IOThread.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef IO_IOTHREAD_HPP_
#define IO_IOTHREAD_HPP_

#include "Connection.hpp"
#include "Thread.hpp"

using namespace Concurrency;
using namespace Networking;

namespace IO {

class IOThread : public Thread {
private:

	/**
	 * Epoll file descriptor
	 */
	int m_epoll_fd;

	/**
	 * Reenables socket descriptor
	 */
	bool reenableSocketDescriptor(int socketd);

	/**
	 * Returns message length
	 */
	int getMessageLength(Connection* connection);

	/**
	 * Returns message control type
	 */
	int getMessageControlType(Connection* connection);

public:

	/**
	 * Constructor
	 */
	IOThread(int epoll_fd) : m_epoll_fd(epoll_fd) {};

	/**
	 * Run
	 */
	void* run();

};

}

#endif /* IO_IOTHREAD_HPP_ */
