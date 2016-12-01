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
		cout << "bind() failed: " << errno << endl;
		return;
	}

	result = listen(m_sd, 5);
	if(result !=0){
		cout << "listen() failed" << endl;
		return;
	}

	cout << "Listening on port " << m_port << endl;

	// Loop indefinitely and accept connections
	while(true){

		cout << "Waiting for incomming connections" << endl;

		// Blocks until connection is returned
		TcpConnection* connection = acceptConnections();

		if(!connection){
			cout << "Could not accept connection" << endl;
			continue;
		}

		cout << "Accepted connection from: " << connection->getPeerIp() << endl;

		// Adding connection to the queue
		m_queue.add(connection);

		cout << "Connection from " << connection->getPeerIp() << " added to the I/0 Queue" << endl;
	}

}

TcpConnection* TcpListener::acceptConnections() {

	// Peer address
	struct sockaddr_in address;
	socklen_t len = sizeof(address);

	memset(&address, 0, len);

	int con_sd = ::accept(m_sd, (struct sockaddr*)&address, &len);

	cout << "Accepted connection on socket " << con_sd << endl;

	if (con_sd < 0) {
		cout << "accept() failed: " << errno << endl;
		return NULL;
	}

	// Create new connection instance and return ptr
	return new TcpConnection(con_sd, &address);

}


TcpListener::~TcpListener(){
	if(m_sd > 0) {
		cout << "Closing socket " << m_sd << endl;
		close(m_sd);
	}
}


