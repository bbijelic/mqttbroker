#ifndef CONFIG_CONFIGURATIONEXCEPTION_H
#define CONFIG_CONFIGURATIONEXCEPTION_H

#include <string>
#include <exception>

namespace Broker {
namespace Config {

/**
 * Configuration exception
 */
class ConfigurationException : public std::exception {
private:

	/**
	 * Exception message
	 */
	std::string m_message;


public:

	/**
	 * Constructor
	 */
	ConfigurationException(std::string message) : m_message(message){};

	/**
	 * Message getter
	 */
	std::string getMessage() { return m_message; };

};

}}


#endif
