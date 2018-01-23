/*
 * TcpConnection.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <iostream>

#include "easylogging++.hpp"
#include "TcpConnection.hpp"

using namespace Networking::TCP;
using namespace std;

TcpConnection::TcpConnection(int socketd, struct sockaddr_in* address){

	this->m_sd = socketd;

	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) &(address->sin_addr.s_addr), ip,
			sizeof(ip) - 1);
	m_peer_ip = ip;
	m_peer_port = ntohs(address->sin_port);
}

TcpConnection::~TcpConnection() {
	closeConnection();
}

ssize_t TcpConnection::send(const char* buffer, size_t length) {
	return write(m_sd, buffer, length);
}

ssize_t TcpConnection::receive(char* buffer, size_t length) {
	return read(m_sd, buffer, length);
}
