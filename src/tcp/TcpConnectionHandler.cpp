/*
 * TcpConnectionHandler.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>

#include "easylogging++.hpp"
#include "TcpConnectionHandler.hpp"

using namespace std;
using namespace TCP;

/**
 * Constructor
 */
TcpConnectionHandler::TcpConnectionHandler(
		TcpConnectionQueue<TcpConnection*>& queue, int epoll_fd) :
		m_queue(queue), m_epoll_fd(epoll_fd) {
}

void* TcpConnectionHandler::run() {

	LOG(INFO)<< "Connection handler thread " << m_tid << " initializing";

	// Remove 1 item at a time and process it.
	// Blocks if no items are available to process
	while (true) {

		// Obtain TCP connection from the queue
		TcpConnection* connection = m_queue.remove();

		// TODO handle connection
		string peer_ip = connection->getPeerIp();

		LOG(INFO) << "Connection handler thread " << m_tid << " handling connection from host " << peer_ip;

		// Obtain connection socket
		int connection_socket = connection->getSocket();

		// Make socket non blocking
		makeSocketNonBlocking(connection_socket);

		// Add socket to the epoll
		registerSocketToEpoll(connection);

		LOG(INFO) << "Connection handler thread " << m_tid << " successfully added connection from " << connection->getPeerIp() << " to the epoll event handler";
	};

	return NULL;

}

bool TcpConnectionHandler::registerSocketToEpoll(TcpConnection *connection) {

	struct epoll_event event;
	event.data.fd = connection->getSocket();
	event.data.ptr = connection;
	event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;

	int result = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, connection->getSocket(),
			&event);

	if (result == -1) {
		LOG(ERROR)<< "epoll_ctl() failed";

		// Close connection
		close(connection->getSocket());

		return false;
	}

	return true;
}

bool TcpConnectionHandler::makeSocketNonBlocking(int socketd) {
	int flags, s;

	flags = fcntl(socketd, F_GETFL, 0);
	if (flags == -1) {
		LOG(ERROR)<< "fcntl() failed";
		return false;
	}

	flags |= O_NONBLOCK;
	s = fcntl(socketd, F_SETFL, flags);
	if (s == -1) {
		LOG(ERROR)<< "fcntl() failed";
		return false;
	}

	return true;
}
