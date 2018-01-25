#ifndef NET_CONNECTOREXCEPTION_H
#define NET_CONNECTOREXCEPTION_H

#include <string>
#include <exception>

namespace Broker {
    namespace Net {

        class ConnectorException : public std::exception {
        private:
            
            // Exception message
            std::string m_message;

        public:

            // Constructor
            ConnectorException(std::string message) : m_message(message) {};

            virtual char const* what() const throw ();

        };
    }
}

#endif