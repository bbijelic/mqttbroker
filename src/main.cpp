/*
 * main.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#include <string>
#include "easylogging++.hpp"

#include "MqttBroker.hpp"

using namespace MQTT;
using namespace std;

INITIALIZE_EASYLOGGINGPP

/**
 * Main
 */
int main(int argc, char** argv) {

	// Load configuration from file
	el::Configurations conf("config/logger.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	// Validate arguments
	if (argc < 3 || argc > 4) {
		LOG(ERROR)<< "usage: " << argv[0] << " <workers> <port> <bind-ip>";
		exit(-1);
	}

	// Workers
	int workers = atoi(argv[1]);

	// Port
	int port = atoi(argv[2]);

	// Bind IP
	string bind_ip;
	if (argc == 4) {
		bind_ip = argv[3];
	}

	// Initialize MQTT broker
	MqttBroker mqtt_broker(workers, port, bind_ip);

	// Blocks while waiting for connections
	mqtt_broker.start();

	return 0;
}

