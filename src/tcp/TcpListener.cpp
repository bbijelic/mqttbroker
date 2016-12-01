/*
 * TcpListener.cpp
 *
 *  Created on: Nov 29, 2016
 *      Author: dev
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>

#include "easylogging++.hpp"
#include "TcpListener.hpp"

using namespace std;
using namespace TCP;

void TcpListener::startListening(){

	// Open socket
	m_sd = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;

	// Set 0 to address
	memset(&address, 0, sizeof(address));

	address.sin_family = PF_INET;
	address.sin_port = htons(m_port);
	address.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	setsockopt(m_sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	// Bind to the interface
	int result = bind(m_sd, (struct sockaddr*) &address, sizeof(address));

	if (result != 0) {
		LOG(ERROR) << "bind() failed: " << errno;
		return;
	}

	result = listen(m_sd, 5);
	if(result !=0){
		LOG(ERROR) << "listen() failed";
		return;
	}

	LOG(INFO) << "Listening on port " << m_port;

	// Loop indefinitely and accept connections
	while(true){

		LOG(INFO) << "Waiting for incomming connections";

		// Blocks until connection is returned
		TcpConnection* connection = acceptConnections();

		if(!connection){
			LOG(ERROR) << "Could not accept connection";
			continue;
		}

		// Adding connection to the queue
		m_queue.add(connection);

		LOG(INFO) << "Connection from " << connection->getPeerIp() << " added to the I0 Queue";
	}

}

TcpConnection* TcpListener::acceptConnections() {

	// Peer address
	struct sockaddr_in address;
	socklen_t len = sizeof(address);

	memset(&address, 0, len);

	int con_sd = ::accept(m_sd, (struct sockaddr*)&address, &len);

	LOG(INFO) << "TCP listener accepted connection on socket " << con_sd;

	if (con_sd < 0) {
		LOG(ERROR) << "accept() failed: " << errno;
		return NULL;
	}

	// Create new connection instance and return ptr
	return new TcpConnection(con_sd, &address);

}


TcpListener::~TcpListener(){
	if(m_sd > 0) {
		LOG(INFO) << "Closing socket " << m_sd;
		close(m_sd);
	}
}


