/*
 * Epoll.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: dev
 */

#include "Epoll.hpp"
#include "easylogging++.hpp"

#include <unistd.h>

using namespace Notification;

Epoll::Epoll() {
	m_epoll_fd = epoll_create1(0);
	if (m_epoll_fd == -1) {
		LOG(ERROR)<< "epoll_create() failed: " << errno;
	}
}

Epoll::~Epoll() {
	if (m_epoll_fd != -1) {
		LOG(DEBUG)<< "Closing epoll file descriptor " << m_epoll_fd;
		close(m_epoll_fd);
	}
}

int Epoll::getDescriptor() {
	return m_epoll_fd;
}

