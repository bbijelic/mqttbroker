#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <iostream>

#include "logging/easylogging++.h"
#include "net/tcp/TcpConnection.h"

Broker::Net::TCP::TcpConnection::TcpConnection(int socketd, struct sockaddr_in* address){

	this->m_sd = socketd;

	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) &(address->sin_addr.s_addr), ip,
			sizeof(ip) - 1);
	m_peer_ip = ip;
	m_peer_port = ntohs(address->sin_port);
}

Broker::Net::TCP::TcpConnection::~TcpConnection() {
	closeConnection();
}

ssize_t Broker::Net::TCP::TcpConnection::send(const char* buffer, size_t length) {
	return write(m_sd, buffer, length);
}

ssize_t Broker::Net::TCP::TcpConnection::receive(char* buffer, size_t length) {
	return read(m_sd, buffer, length);
}