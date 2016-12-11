/*
 * ConfigurationException.hpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#ifndef MQTT_MQTTSERVEREXCEPTION_HPP_
#define MQTT_MQTTSERVEREXCEPTION_HPP_

#include <string>
#include <exception>

using namespace std;

namespace MQTT {

/**
 * MQTT server exception
 */
class MqttServerException : public exception {
private:

	/**
	 * Exception message
	 */
	string m_message;


public:

	/**
	 * Constructor
	 */
	MqttServerException(string message) : m_message(message){};

	/**
	 * Message getter
	 */
	string getMessage() { return m_message; };

};

}


#endif /* MQTT_MQTTSERVEREXCEPTION_HPP_ */
