/*
 * TcpConnectionQueueItem.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include "TcpConnectionQueueItem.hpp"

using namespace TCP;

TcpConnectionQueueItem::TcpConnectionQueueItem(TcpConnection* connection) :
		m_tcp_connection(connection) {
}

TcpConnectionQueueItem::~TcpConnectionQueueItem() {
	delete m_tcp_connection;
}

TcpConnection* TcpConnectionQueueItem::getConnection() {
	return m_tcp_connection;
}

