#include "net/tcp/TcpSocketException.h"

const char * Broker::Net::TCP::TcpSocketException::what() const throw () {
    return m_message.c_str();
}



