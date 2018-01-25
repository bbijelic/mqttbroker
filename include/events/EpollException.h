#ifndef EVENTS_EPOLLEXCEPTION_H
#define EVENTS_EPOLLEXCEPTION_H

#include <string>
#include <exception>

namespace Broker {
    namespace Events {

        class EpollException : public std::exception {
        private:
            
            // Exception message
            std::string m_message;

        public:

            // Constructor
            EpollException(std::string message) : m_message(message) {};

            virtual char const* what() const throw ();

        };
    }
}

#endif