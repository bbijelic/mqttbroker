#include "net/Connection.h"
#include "logging/easylogging++.h"

#include <unistd.h>
#include <string>

std::string Broker::Net::Connection::getPeerIp() {
    return m_peer_ip;
}

int Broker::Net::Connection::getPeerPort() {
    return m_peer_port;
}

void Broker::Net::Connection::close() {

    if (m_is_closed) {
        LOG(DEBUG) << "Connection to client " << m_peer_ip << " already closed";
        return;
    }

    /* Close the socket */
    int close_result = ::close(m_descriptor);
    if (close_result == 0) {

        LOG(DEBUG) << "Closed connection to client " << m_peer_ip;

        /* Socket closed successfully */
        m_is_closed = true;

    } else {

        /* An error has occured */
        /* Retrying failed close call is a no-no; check close() man NOTES */

        switch (errno) {
            case EBADF: LOG(ERROR) << "Socket descriptor is not a valid descriptor";
            case EINTR: LOG(ERROR) << "Close operation has ben interrupted by a signal";
            case EIO: LOG(ERROR) << "An I/O error has occurred";
        }

    }
}

bool Broker::Net::Connection::isClosed() {
    return m_is_closed;
}
