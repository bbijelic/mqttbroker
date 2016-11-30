/*
 * IOThread.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef TCP_IOTHREAD_HPP_
#define TCP_IOTHREAD_HPP_

#include "TcpConnection.hpp"
#include "Thread.hpp"

using namespace Concurrency;

namespace TCP {

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

#endif /* TCP_IOTHREAD_HPP_ */
