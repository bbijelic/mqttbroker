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

void Broker::Net::Connection::closeConnection() {
	LOG(INFO)<< "Closing connection from " << m_peer_ip << " on socket " << m_descriptor;
	if(close(m_descriptor) == 0) m_is_closed = true;
}

bool Broker::Net::Connection::isClosed() {
    return m_is_closed;
}
