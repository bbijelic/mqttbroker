/*
 * WorkerPoolConfiguration.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#include "WorkerPoolConfiguration.hpp"
#include "easylogging++.hpp"

using namespace Configuration;

WorkerPoolConfiguration::~WorkerPoolConfiguration(){
	LOG(DEBUG) << "Destroying IO worker pool configuration instance";
}

int WorkerPoolConfiguration::getMinPoolSize(){
	return m_min_size;
}

int WorkerPoolConfiguration::getMaxPoolSize(){
	return m_max_size;
}

