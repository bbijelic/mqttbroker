/*
 * IOThread.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: dev
 */

#include "IOThread.hpp"
#include "Connection.hpp"
#include "ControlPacketType.h"
#include "ConnectControlPacketParser.h"
#include "ControlPacketFixedHeader.h"
#include "ConnackControlPacketProducer.h"
#include "easylogging++.hpp"

#include <sys/epoll.h>
#include <stdlib.h>

#define MAXEVENTS 64

using namespace std;
using namespace Networking;
using namespace IO;
using namespace MQTT::Control;
using namespace MQTT::Control::Connect;
using namespace MQTT::Control::Connack;

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

						// Handle message and return thr CONNACK
						ConnectControlPacket* connectControlPacket = ConnectControlPacketParser::parse(msgbuff);
						
						// Connack control package fixed header
						ControlPacketFixedHeader* connack_fixed_header = new ControlPacketFixedHeader();
						connack_fixed_header->control_packet_type = MqttControlPacketType::CONNACK;
						connack_fixed_header->remaining_length = 2;
						
						// Connect acknowledge flags
						ConnectAcknowledgeFlags* connect_ack_flags = new ConnectAcknowledgeFlags();
						connect_ack_flags->session_present = 0;
						
						// Connack control package variable header
						ConnackVariableHeader* connack_variable_header = new ConnackVariableHeader();
						connack_variable_header->acknowledge_flags = connect_ack_flags;
						connack_variable_header->connect_return_code = ConnectReturnCode::CONNECTION_ACCEPTED;
						
						// Initialize connack control package
						ConnackControlPacket* connack_control_package = new ConnackControlPacket();
						connack_control_package->fixed_header = connack_fixed_header;
						connack_control_package->variable_header = connack_variable_header;
						
						char* connack_bytes = ConnackControlPacketProducer::serialize(connack_control_package);
						// Send CONNACK to the client
						connection->send(connack_bytes, sizeof connack_bytes);
						
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

