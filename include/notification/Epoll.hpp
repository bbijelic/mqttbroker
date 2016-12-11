/*
 * Epoll.hpp
 *
 *  Created on: Dec 9, 2016
 *      Author: dev
 */

#ifndef EPOLL_EPOLL_HPP_
#define EPOLL_EPOLL_HPP_

#include <sys/epoll.h>

namespace Notification {

class Epoll {
private:

	/**
	 * Epoll file descriptor
	 */
	int m_epoll_fd;

public:

	/**
	 * Constructor
	 */
	Epoll();

	/**
	 * Destructor
	 */
	~Epoll();

	/**
	 * Returns epoll file descriptor
	 */
	int getDescriptor();
};

}


#endif /* EPOLL_EPOLL_HPP_ */
