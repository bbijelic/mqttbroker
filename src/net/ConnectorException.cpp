#include "net/ConnectorException.h"

const char * Broker::Net::ConnectorException::what() const throw () {
    return m_message.c_str();
}



