#include "events/Epoll.h"
#include "events/EpollException.h"
#include "logging/easylogging++.h"

#include <sys/epoll.h>
#include <unistd.h>

/**
 * Constructor
 * @param epoll_name the name of the epoll instance
 */
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

void Broker::Events::Epoll::addDescriptor(int file_descriptor, unsigned int events) {
    epoll_event event;
    event.events = events;
    event.data.fd = file_descriptor;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, file_descriptor, &event) == -1) {
        throw Broker::Events::EpollException("Failed to add file descriptor to the epoll");
    }
    LOG(DEBUG) << "Added file descriptor to epoll interest list " << file_descriptor;
}