#include "config/TcpListenerConfiguration.h"

#include <string>

Broker::Config::TcpListenerConfiguration::~TcpListenerConfiguration(){

}

int Broker::Config::TcpListenerConfiguration::getPort(){
	return m_port;
};

std::string Broker::Config::TcpListenerConfiguration::getBindAddress(){
	return m_bind_address;
};


