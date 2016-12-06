/*
 * SslListenerConfiguration.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#include <string>
#include "SslListenerConfiguration.hpp"
#include "easylogging++.hpp"

using namespace std;
using namespace Configuration;

SslListenerConfiguration::~SslListenerConfiguration(){
	LOG(DEBUG) << "Destroying SSL Listener configuration instance";
}

int SslListenerConfiguration::getPort(){
	return m_port;
}

string SslListenerConfiguration::getBindAddress(){
	return m_bind_address;
}

string SslListenerConfiguration::getCaCertFile(){
	return m_ca_certfile;
}

string SslListenerConfiguration::getServerCertFile(){
	return m_server_certfile;
}

string SslListenerConfiguration::getServerKeyFile(){
	return m_server_keyfile;
}

bool SslListenerConfiguration::getVerifyPeer(){
	return m_verify_peer;
}


