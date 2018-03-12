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
            case EBADF: LOG(ERROR) << CLOSE_EBADF_MSG;
            case EINTR: LOG(ERROR) << CLOSE_EINTR_MSG;
            case EIO: LOG(ERROR) << CLOSE_EIO_MSG;
        }

    }
}

bool Broker::Net::Connection::isClosed() {
    return m_is_closed;
}

/* Returns reference to the inbound message buffer */
Broker::Net::MessageBuffer& Broker::Net::Connection::getInboundMessageBuffer(){
    return m_inbound_message_buffer;
}

/* Returns reference to the outbound message buffer */
Broker::Net::MessageBuffer& Broker::Net::Connection::getOutboundMessageBuffer(){
    return m_outbound_message_buffer;
}
