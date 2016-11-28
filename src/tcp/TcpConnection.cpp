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
#include <TcpConnection.hpp>
#include <TcpConnectionE.hpp>

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

ssize_t TcpConnection::receive(char* buffer, size_t length, int timeout) {
	if (timeout <= 0)
		return read(m_sd, buffer, length);

	if (waitForReadEvent(timeout) == true) {
		return read(m_sd, buffer, length);
	}

	return TcpConnectionE::CONNECTION_TIMED_OUT;
}

string TcpConnection::getPeerIp() {
	return m_peer_ip;
}

int TcpConnection::getPeerPort() {
	return m_peer_port;
}

bool TcpConnection::waitForReadEvent(int timeout) {
	fd_set sdset;
	struct timeval tv;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	FD_ZERO(&sdset);
	FD_SET(m_sd, &sdset);
	if (select(m_sd + 1, &sdset, NULL, NULL, &tv) > 0) {
		return true;
	}

	return false;
}
