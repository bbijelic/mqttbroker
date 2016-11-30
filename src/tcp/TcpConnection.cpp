/*
 * TcpConnection.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/select.h>
#include <sys/time.h>

#include "TcpConnection.hpp"

using namespace TCP;
using namespace std;

TcpConnection::TcpConnection(int socketd, struct sockaddr_in* address) :
		m_sd(socketd) {
	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) &(address->sin_addr.s_addr), ip,
			sizeof(ip) - 1);
	m_peer_ip = ip;
	m_peer_port = ntohs(address->sin_port);
}

TcpConnection::~TcpConnection() {
	close(m_sd);
}

ssize_t TcpConnection::send(const char* buffer, size_t length) {
	return write(m_sd, buffer, length);
}

ssize_t TcpConnection::receive(char* buffer, size_t length) {
	return read(m_sd, buffer, length);
}

string TcpConnection::getPeerIp() {
	return m_peer_ip;
}

int TcpConnection::getPeerPort() {
	return m_peer_port;
}

int TcpConnection::getSocket(){
	return m_sd;
}
