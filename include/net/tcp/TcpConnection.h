#ifndef NET_TCP_TCPCONNECTION_H
#define NET_TCP_TCPCONNECTION_H

#include "net/Connection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>

namespace Broker {
    namespace Net {
        namespace TCP {

            /**
             * Tcp Connection
             */
            class TcpConnection : public Broker::Net::Connection {
            public:

                /**
                 * Constructor
                 */
                TcpConnection(int, std::string, int);

                /**
                 * Constructor
                 */
                TcpConnection(const TcpConnection&);

                /**
                 * Destructor
                 */
                ~TcpConnection();

                /**
                 * Sends via socket
                 */
                ssize_t send(const char* buffer, size_t length);

                /**
                 * Receive from the socket
                 */
                ssize_t receive(char* buffer, size_t length);

            };

        }
    }
}

#endif