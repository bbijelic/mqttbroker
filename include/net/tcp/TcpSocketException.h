#ifndef NET_TCP_TCPSOCKETEXCEPTION_H
#define NET_TCP_TCPSOCKETEXCEPTION_H

#include <string>
#include <exception>

namespace Broker {
    namespace Net {
        namespace TCP {

            class TcpSocketException : public std::exception {
            private:

                // Exception message
                std::string m_message;

            public:

                // Constructor

                TcpSocketException(std::string message) : m_message(message) {
                };

                virtual char const* what() const throw ();

            };
        }
    }
}

#endif