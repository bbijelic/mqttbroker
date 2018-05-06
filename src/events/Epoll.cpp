#include "events/Epoll.h"
#include "events/EpollException.h"
#include "logging/easylogging++.h"
#include "sys/Descriptor.h"

#include <sys/epoll.h>
#include <unistd.h>

/**
 * Constructor
 * @param epoll_name the name of the epoll instance
 */
Broker::Events::Epoll::Epoll(std::string epoll_name) {
    m_epoll_name = epoll_name;
    m_descriptor = epoll_create1(0);
    if (m_descriptor == -1) {
        LOG(ERROR) << "epoll_create() failed: " << errno;
    }
    LOG(DEBUG) << "Epoll instance created: " << m_epoll_name;
}

Broker::Events::Epoll::~Epoll() {
    if (m_descriptor != -1) {
        LOG(DEBUG) << "Closing epoll " << m_epoll_name;
        close(m_descriptor);
    }
}

std::string Broker::Events::Epoll::getName() {
    return m_epoll_name;
}

void Broker::Events::Epoll::setMaxEvents(unsigned int max_events) {
    m_max_events = max_events;
}

const unsigned int Broker::Events::Epoll::getMaxEvents() {
    return m_max_events;
}

void Broker::Events::Epoll::add(unsigned int events, Broker::SYS::Descriptor* descriptor) {
    epoll_event event;
    event.events = events;
    event.data.ptr = descriptor;
    if (epoll_ctl(m_descriptor, EPOLL_CTL_ADD, descriptor->getDescriptor(), &event) == -1) {
        throw Broker::Events::EpollException(
                "Failed to add file descriptor to the epoll instance '" + m_epoll_name + "'");
    }
    LOG(DEBUG) << "Added file descriptor to epoll interest list '" << m_epoll_name << "': " << descriptor->getDescriptor();
}

void Broker::Events::Epoll::add(unsigned int events, Broker::Net::Connection* connection) {
    epoll_event event;
    event.events = events;
    event.data.ptr = connection;
    if (epoll_ctl(m_descriptor, EPOLL_CTL_ADD, connection->getDescriptor(), &event) == -1) {
        throw Broker::Events::EpollException(
                "Failed to add file descriptor to the epoll instance '" + m_epoll_name + "'");
    }
    LOG(DEBUG) << "Added file descriptor to epoll interest list '" << m_epoll_name << "': " << connection->getDescriptor();
}

void Broker::Events::Epoll::modify(int descriptor, epoll_event& event) {
    if (epoll_ctl(m_descriptor, EPOLL_CTL_MOD, descriptor, &event) == -1) {        
        throw Broker::Events::EpollException(
                "Failed to modify descriptor on the epoll interest list '" + m_epoll_name + "': " + std::to_string(errno));
    }
    LOG(DEBUG) << "Descriptor successfully modified on epoll '" << m_epoll_name << "' interest list: " << descriptor;
}


void Broker::Events::Epoll::remove(int descriptor) {
    if (epoll_ctl(m_descriptor, EPOLL_CTL_DEL, descriptor, NULL) == -1) {
        throw Broker::Events::EpollException(
                "Failed to delete descriptor from the epoll interest list '" + m_epoll_name + "'");
    }
    LOG(DEBUG) << "Descriptor successfully deleted from epoll '" << m_epoll_name << "' interest list: " << descriptor;
}