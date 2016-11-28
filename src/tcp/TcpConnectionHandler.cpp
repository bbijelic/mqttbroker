/*
 * TcpConnectionHandler.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include "easylogging++.hpp"

#include "TcpConnectionHandler.hpp"
#include "MqttControlPacketType.hpp"

using namespace MQTT;
using namespace TCP;

/**
 * Constructor
 */
TcpConnectionHandler::TcpConnectionHandler(
		TcpConnectionQueue<TcpConnectionQueueItem*>& queue) :
		m_queue(queue) {
}


void* TcpConnectionHandler::run() {

	LOG(INFO)<< "Running TCP connection handler";

	// Remove 1 item at a time and process it.
	// Blocks if no items are available to process
	while (true) {

		// Obtain TCP connection queue item from the queue
		TcpConnectionQueueItem* queue_item = m_queue.remove();

		// Obtain TCP connection from the queue item
		TcpConnection* connection = queue_item->getConnection();

		// TODO handle connection
		string peer_ip = connection->getPeerIp();

		LOG(INFO) << "Handling connection from host '" << peer_ip << "'";

		// Fixed header bytes
		char fixedHeader[1];

		// Get first two bytes
		int fixedHeadeRcvSize = connection->receive(fixedHeader, sizeof(fixedHeader));

		if(fixedHeadeRcvSize != sizeof(fixedHeader)) {
			LOG(ERROR) << "Cant read MQTT fixed header";
		}

		int mqttControlType = fixedHeader[0] >> 4;
		if(mqttControlType < 1 || mqttControlType > 14) {
			LOG(ERROR) << "Unknown MQTT control type";
		}

		if(mqttControlType == MqttControlPacketType::CONNECT) {
			LOG(INFO) << "MQTT Control Type: CONNECT";
		}

		// Deletes queue item which also kills the connection
		delete queue_item;

	};

	return NULL;

}
