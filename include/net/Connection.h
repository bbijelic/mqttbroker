#ifndef NETWORKING_CONNECTION_H
#define NETWORKING_CONNECTION_H

#include "sys/Descriptor.h"

#include <string>

namespace Broker {
    namespace Net {

        /**
         * Connection interface
         */
        class Connection : public Broker::SYS::Descriptor {
        protected:

            /**
             * Peer IP address
             */
            std::string m_peer_ip;

            /**
             * Peer port
             */
            int m_peer_port;

            /**
             * Is connection closed
             */
            bool m_is_closed = false;

        public:
                       
            /**
             * Returns peer ip address
             */
            std::string getPeerIp();

            /**
             * Returns peer port
             */
            int getPeerPort();

            /**
             * Sends via socket
             */
            virtual ssize_t send(const char* buffer, size_t length) = 0;

            /**
             * Receive from the socket
             */
            virtual ssize_t receive(char* buffer, size_t length) = 0;

            /**
             * Closes connection
             */
            void closeConnection();

            /**
             * Is connection closed
             */
            bool isClosed();

        };

    }
}

#endif