#ifndef NET_TCP_TCPCONNECTOR_H
#define NET_TCP_TCPCONNECTOR_H

#include "events/Epoll.h"

#include <string>
#include <memory>

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
                
                // Epoll file descriptor
                std::shared_ptr<Broker::Events::Epoll> m_epoll;
                
                /* Is connector running */
                /* State is changed within start and stop functions */
                bool m_is_running = false;

            public:

                /**
                 * Constructor
                 */
                TcpConnector(int, std::string, const std::shared_ptr<Broker::Events::Epoll>);

                /**
                 * Destructor
                 */
                ~TcpConnector();
                
                void start();
                
                void stop();

            };

        }
    }
}

#endif