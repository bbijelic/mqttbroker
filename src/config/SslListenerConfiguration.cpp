#include "config/SslListenerConfiguration.h"

#include <string>

Broker::Config::SslListenerConfiguration::~SslListenerConfiguration(){
	
}

int Broker::Config::SslListenerConfiguration::getPort(){
	return m_port;
}

std::string Broker::Config::SslListenerConfiguration::getBindAddress(){
	return m_bind_address;
}

std::string Broker::Config::SslListenerConfiguration::getCaCertFile(){
	return m_ca_certfile;
}

std::string Broker::Config::SslListenerConfiguration::getServerCertFile(){
	return m_server_certfile;
}

std::string Broker::Config::SslListenerConfiguration::getServerKeyFile(){
	return m_server_keyfile;
}

bool Broker::Config::SslListenerConfiguration::getVerifyPeer(){
	return m_verify_peer;
}


