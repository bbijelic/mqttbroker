/*
 * Connection.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: dev
 */

#include "Connection.hpp"
#include "easylogging++.hpp"

#include <unistd.h>

using namespace Networking;

string Connection::getPeerIp() {
	return m_peer_ip;
}

int Connection::getPeerPort() {
	return m_peer_port;
}

int Connection::getSocket() {
	return m_sd;
}

void Connection::closeConnection() {
	LOG(INFO)<< "Closing connection from " << m_peer_ip << " on socket " << m_sd;
	close(m_sd);
}

