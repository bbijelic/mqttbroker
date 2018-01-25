#include "net/Connection.h"
#include "logging/easylogging++.h"

#include <unistd.h>

std::string Broker::Net::Connection::getPeerIp() {
	return m_peer_ip;
}

int Broker::Net::Connection::getPeerPort() {
	return m_peer_port;
}

int Broker::Net::Connection::getSocket() {
	return m_sd;
}

void Broker::Net::Connection::closeConnection() {
	LOG(INFO)<< "Closing connection from " << m_peer_ip << " on socket " << m_sd;
	if(close(m_sd) == 0) m_is_closed = true;
}

bool Broker::Net::Connection::isClosed() {
    return m_is_closed;
}
