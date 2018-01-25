#include "events/EpollException.h"

const char * Broker::Events::EpollException::what() const throw () {
    return m_message.c_str();
}



