#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <iostream>
#include <string>

#include "logging/easylogging++.h"
#include "net/tcp/TcpConnection.h"

Broker::Net::TCP::TcpConnection::TcpConnection(int socketd, std::string ip, int remote_port){

	this->m_descriptor = socketd;
	m_peer_ip = ip;
	m_peer_port = remote_port;
}

Broker::Net::TCP::TcpConnection::~TcpConnection() {
	closeConnection();
}

ssize_t Broker::Net::TCP::TcpConnection::send(const char* buffer, size_t length) {
	return write(m_descriptor, buffer, length);
}

ssize_t Broker::Net::TCP::TcpConnection::receive(char* buffer, size_t length) {
	return read(m_descriptor, buffer, length);
}