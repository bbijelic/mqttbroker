/*
 * TcpListenerConfiguration.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#include "TcpListenerConfiguration.hpp"
#include <easylogging++.hpp>
#include <string>

using namespace std;
using namespace Configuration;

TcpListenerConfiguration::~TcpListenerConfiguration(){
	LOG(DEBUG) << "Destroying TCP Listener configuration instance";
}

int TcpListenerConfiguration::getPort(){
	return m_port;
};

string TcpListenerConfiguration::getBindAddress(){
	return m_bind_address;
};


