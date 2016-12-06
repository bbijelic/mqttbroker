/*
 * ConfigurationException.hpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#ifndef CONFIG_CONFIGURATIONEXCEPTION_HPP_
#define CONFIG_CONFIGURATIONEXCEPTION_HPP_

#include <string>
#include <exception>

using namespace std;

namespace Configuration {

/**
 * Configuration exception
 */
class ConfigurationException : public exception {
private:

	/**
	 * Exception message
	 */
	string m_message;


public:

	/**
	 * Constructor
	 */
	ConfigurationException(string message) : m_message(message){};

	/**
	 * Message getter
	 */
	string getMessage() { return m_message; };

};

}


#endif /* CONFIG_CONFIGURATIONEXCEPTION_HPP_ */
