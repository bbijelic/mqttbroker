#ifndef NETWORKING_CONNECTION_H
#define NETWORKING_CONNECTION_H

#include "sys/Descriptor.h"
#include "MessageBuffer.h"

#include <string>

#define CLOSE_EBADF_MSG "Socket descriptor is not a valid descriptor"
#define CLOSE_EINTR_MSG "Close operation has ben interrupted by a signal"
#define CLOSE_EIO_MSG   "An I/O error has occurred"

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
            
            /**
             * Inbound message buffer
             */
            MessageBuffer m_inbound_message_buffer;            

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
            void close();

            /**
             * Is connection closed
             */
            bool isClosed();
            
            /* Returns reference to the message buffer */
            MessageBuffer& getInboundMessageBuffer();

        };

    }
}

#endif