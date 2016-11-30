/*
 * IOThread.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: dev
 */

#include "IOThread.hpp"
#include "TcpConnection.hpp"

#include <sys/epoll.h>
#include <iostream>
#include <stdlib.h>

#define MAXEVENTS 1

using namespace std;
using namespace TCP;

void* IOThread::run() {

	struct epoll_event *events = (epoll_event *) calloc(MAXEVENTS,
			sizeof(struct epoll_event));

	while (true) {

		// Get epoll events
		int num_events = epoll_wait(m_epoll_fd, events, MAXEVENTS, -1);

		cout << "IO thread " << m_tid << " got " << num_events << " events"
				<< endl;

		for (int i = 0; i < num_events; i++) {

			// Get tcp connection from event data pointer
			TcpConnection* connection = (TcpConnection*) events[i].data.ptr;

			cout << "IO thread" << m_tid << " handles connection from socket "
					<< events[i].data.fd << endl;

			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
					|| (events[i].events & EPOLLRDHUP)
					|| (!(events[i].events & EPOLLIN))) {

				/* An error has occured on this fd, or the socket is not
				 ready for reading (why were we notified then?) */
				cout << "IO thread " << m_tid
						<< " handling error on epoll, closing socket "
						<< events[i].data.fd << endl;

				/* Closing the descriptor will make epoll remove it
				 from the set of descriptors which are monitored. */
				close(events[i].data.fd);

				continue;

			} else {

				/* We have data on the socket waiting to be read. Read and
				 display it. We must read whatever data is available
				 completely, as we are running in edge-triggered mode
				 and won't get a notification again for the same
				 data. */

				cout << "IO thread " << m_tid << " handling data from "
						<< connection->getPeerIp() << " on socket " << events[i].data.fd << endl;

				char buffer[500];
				int received = connection->receive(buffer, sizeof buffer);

				cout << "IO thread " << m_tid << " received " << received
						<< " bytes from socket " << events[i].data.fd << endl;

				// Reset what we are watching for
				events[i].events = EPOLLIN | EPOLLET | EPOLLONESHOT;
				// tell epoll to re-enable this fd.
				epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, events[i].data.fd,
						&events[i]);
			}

		}
	}

}

