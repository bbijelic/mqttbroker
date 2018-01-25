#ifndef NET_TCP_TCPCONNECTOR_H
#define NET_TCP_TCPCONNECTOR_H

#include "events/Epoll.h"

#include <string>

namespace Broker {
    namespace Net {
        namespace TCP {

            /**
             * TCP Connector
             */
            class TcpConnector {
            private:

                // Port number
                int m_port;

                // Binding address
                std::string m_bind_address;

                // Socket descriptor
                int m_socket_descriptor;

                // Socket epoll instance
                Broker::Events::Epoll* m_socket_epoll;


            public:

                /**
                 * Constructor
                 */
                TcpConnector(int, std::string, Broker::Events::Epoll*);

                /**
                 * Destructor
                 */
                ~TcpConnector();

            };

        }
    }
}

#endif