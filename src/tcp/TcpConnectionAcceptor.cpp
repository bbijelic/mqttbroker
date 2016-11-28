/*
 * TcpConnectionAcceptor.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "easylogging++.hpp"
#include <TcpConnectionAcceptor.hpp>

using namespace TCP;
using namespace std;

TcpConnectionAcceptor::TcpConnectionAcceptor(int port, const char* address) : m_lsd(0), m_port(port), m_address(address), m_listening(false) {}

TcpConnectionAcceptor::~TcpConnectionAcceptor() {
	if (m_lsd > 0)
		close(m_lsd);
}

int TcpConnectionAcceptor::start() {
	// Already listening?
	if (m_listening == true)
		return 0;

	// Open socket
	m_lsd = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;

	// Set 0 to address
	memset(&address, 0, sizeof(address));

	address.sin_family = PF_INET;
	address.sin_port = htons(m_port);

	if (m_address.size() > 0) {
		// Convert IPv4 and IPv6 addresses from text to binary form
		inet_pton(PF_INET, m_address.c_str(), &(address.sin_addr));
	} else {
		address.sin_addr.s_addr = INADDR_ANY;
	}

	int optval = 1;

	// TODO What does this function do?
	setsockopt(m_lsd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	// Bind to the interface
	int result = bind(m_lsd, (struct sockaddr*) &address, sizeof(address));

	if (result != 0) {
		LOG(ERROR) << "bind() failed";
		return result;
	}

	result = listen(m_lsd, 5);
	if(result !=0){
		LOG(ERROR) << "listen() failed";
		return result;
	}

	LOG(INFO) << "Listening on port " << m_port;

	m_listening = true;
	return result;
}

TcpConnection* TcpConnectionAcceptor::accept() {

	// Not listening, return null
	if (m_listening == false)
		return NULL;

	// Peer address
	struct sockaddr_in address;
	socklen_t len = sizeof(address);

	memset(&address, 0, len);

	int sd = ::accept(m_lsd, (struct sockaddr*)&address, &len);

	if (sd < 0) {
		LOG(ERROR) << "accept() failed";
		return NULL;
	}

	return new TcpConnection(sd, &address);

}
