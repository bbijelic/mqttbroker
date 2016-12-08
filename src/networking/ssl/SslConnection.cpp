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
#include "SslConnection.hpp"

#include <openssl/ssl.h>

using namespace Networking::Security;
using namespace std;

SslConnection::SslConnection(SSL* ssl, int socketd, struct sockaddr_in* address) {

	this->m_ssl = ssl;
	this->m_sd = socketd;

	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) &(address->sin_addr.s_addr), ip,
			sizeof(ip) - 1);
	m_peer_ip = ip;
	m_peer_port = ntohs(address->sin_port);
}

SslConnection::~SslConnection() {
	closeConnection();
}

ssize_t SslConnection::send(const char* buffer, size_t length) {
	return SSL_write(m_ssl, buffer, length);

}

ssize_t SslConnection::receive(char* buffer, size_t length) {
	LOG(DEBUG) << "Receaving data from SSL socket";
	return SSL_read(m_ssl, buffer, length);;
}
