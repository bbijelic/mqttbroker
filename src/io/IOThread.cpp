/*
 * IOThread.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: dev
 */

#include "IOThread.hpp"
#include "Connection.hpp"
#include "MqttControlPacketType.hpp"
#include "ConnectControlPacketParser.h"
#include "easylogging++.hpp"

#include <sys/epoll.h>
#include <stdlib.h>

#define MAXEVENTS 64

using namespace std;
using namespace Networking;
using namespace IO;
using namespace MQTT::Control;
using namespace MQTT::Control::Connect;

int IOThread::getMessageControlType(Connection* connection) {

	// Read only one byte, which is message control byte
	char msgtype[1];
	int received = connection->receive(msgtype, 1);

	if (received == 0) {
		// Client closed connection

		LOG(INFO) << "IO thread " << m_tid
				<< " handling closed connection from client "
				<< connection->getPeerIp() << " on socket "
				<< connection->getSocket();

		// Close connection
		connection->closeConnection();

		// Continue event loop
		return -1;
	}

	// Shift bytes to get message type
	return msgtype[0] >> 4;
}

int IOThread::getMessageLength(Connection* connection) {

	int multiplier = 1;
	int length = 0;

	char buffer[1];
	do {
		// Get next byte of the stream
		int received = connection->receive(buffer, sizeof buffer);

		if (received == 0) {
			// Client closed connection

			LOG(INFO) << "IO thread " << m_tid
					<< " handling closed connection from client "
					<< connection->getPeerIp() << " on socket "
					<< connection->getSocket();

			// Close connection
			connection->closeConnection();

			// Continue event loop
			return -1;
		}

		length += (buffer[0] & 127) * multiplier;
		multiplier *= 128;

		// Malformed remaining length
		if (multiplier > 128 * 128 * 128)
			return -1;

	} while ((buffer[0] & 128) != 0);

	// Return length
	return length;
}

void* IOThread::run() {

	struct epoll_event *events = (epoll_event *) calloc(MAXEVENTS,
			sizeof(struct epoll_event));

	while (!m_graceful_stop) {

		// Get epoll events
		int num_events = epoll_wait(m_epoll_fd, events, MAXEVENTS, -1);

		LOG(INFO) << "IO thread " << m_tid << " handling " << num_events << " events";


		for (int i = 0; i < num_events; i++) {

			// Get tcp connection from event data pointer
			Connection* connection = (Connection*) events[i].data.ptr;

			LOG(INFO) << "IO thread " << m_tid << " handles connection from socket "
					<< connection->getSocket();

			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
					|| (events[i].events & EPOLLRDHUP)
					|| (!(events[i].events & EPOLLIN))) {

				/* An error has occured on this fd, or the socket is not
				 ready for reading (why were we notified then?) */
				LOG(ERROR) << "IO thread " << m_tid
						<< " handling error on epoll, closing socket "
						<< connection->getSocket();

				/* Closing the descriptor will make epoll remove it
				 from the set of descriptors which are monitored. */
				connection->closeConnection();

				continue;

			} else {

				/* We have data on the socket waiting to be read. Read and
				 display it. We must read whatever data is available
				 completely, as we are running in edge-triggered mode
				 and won't get a notification again for the same
				 data. */

				LOG(INFO) << "IO thread " << m_tid << " handling data from "
						<< connection->getPeerIp() << " on socket "
						<< connection->getSocket();

				// Get message type
				int msg_type = getMessageControlType(connection);

				// Get message connection
				int message_length = getMessageLength(connection);
				if (message_length == -1) {

					LOG(ERROR) << "IO Thread " << m_tid
							<< " could not get message length from client "
							<< connection->getPeerIp() << " on socket "
							<< connection->getSocket();

					// Close connection
					connection->closeConnection();

					continue;

				} else {

					LOG(INFO) << "IO Thread " << m_tid << " obtaining total of "
							<< message_length << " message bytes from "
							<< connection->getPeerIp() << " on socket "
							<< connection->getSocket();

					char msgbuff[message_length];

					int bytes_received = 0;
					bool handleNextEvent = false;

					while (bytes_received < message_length) {

						// Receive bytes
						int rcvd = connection->receive(msgbuff, message_length);

						if (rcvd == 0) {
							LOG(INFO) << "IO thread " << m_tid
									<< " handling closed connection from client "
									<< connection->getPeerIp() << " on socket "
									<< connection->getSocket();

							// Close connection
							connection->closeConnection();

							handleNextEvent = true;
							break;
						}

						bytes_received += rcvd;

					};

					if (handleNextEvent) {
						// Continue to next event
						continue;
					}

					if (msg_type == MqttControlPacketType::CONNECT) {
						// Handling CONNECT message

						LOG(INFO) << "IO Thread " << m_tid
								<< " handling CONNECT message from client "
								<< connection->getPeerIp() << " on socket "
								<< connection->getSocket();

						// Handle message
						ConnectControlPacket* connectControlPacket = ConnectControlPacketParser::parse(msgbuff);
					}

				}

				// Reset what we are watching for
				events[i].events = EPOLLIN | EPOLLET | EPOLLONESHOT;
				// tell epoll to re-enable this fd.
				int result = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD,
						connection->getSocket(), &events[i]);

				if (result == -1) {
					LOG(ERROR) << "epoll_ctl() failed";

					// Close connection
					connection->closeConnection();

					continue;
				}

			}

		}
	}

	return NULL;

}

