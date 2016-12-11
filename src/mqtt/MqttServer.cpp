/*
 * MqttServer.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: dev
 */

#include "easylogging++.hpp"
#include "MqttServer.hpp"

using namespace MQTT;

MqttServer::~MqttServer(){
	delete m_epoll;
	delete m_config;
}

void MqttServer::startServer(){
	LOG(INFO)<< "Starting MQTT broker node: " << m_config->getNodeName();
}

void MqttServer::stopServer(){
	LOG(INFO)<< "Stopping MQTT broker node: " << m_config->getNodeName();
}


