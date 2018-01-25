#include "events/Epoll.h"
#include "logging/easylogging++.h"

#include <unistd.h>

Broker::Events::Epoll::Epoll(std::string epoll_name) {
    m_epoll_name = epoll_name;
	m_epoll_fd = epoll_create1(0);
	if (m_epoll_fd == -1) {
		LOG(ERROR) << "epoll_create() failed: " << errno;
	}
	LOG(DEBUG) << "Epoll instance created: " << m_epoll_name;
}

Broker::Events::Epoll::~Epoll() {
	if (m_epoll_fd != -1) {
		LOG(DEBUG) << "Closing epoll file descriptor " << m_epoll_fd;
		close(m_epoll_fd);
	}
}

const int Broker::Events::Epoll::getDescriptor() {
	return m_epoll_fd;
}

std::string Broker::Events::Epoll::getName() {
	return m_epoll_name;
}