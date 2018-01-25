#include "config/WorkerPoolConfiguration.h"

Broker::Config::WorkerPoolConfiguration::~WorkerPoolConfiguration(){
	
}

int Broker::Config::WorkerPoolConfiguration::getMinPoolSize(){
	return m_min_size;
}

int Broker::Config::WorkerPoolConfiguration::getMaxPoolSize(){
	return m_max_size;
}

