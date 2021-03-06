#ifndef NETWORKING_CONNECTION_H
#define NETWORKING_CONNECTION_H

#include "sys/Descriptor.h"
#include "MessageBuffer.h"

#include <string>
#include <queue>

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

            /**
             * Outbound message buffer
             */
            MessageBuffer m_outbound_message_buffer;

            /* Message queue containing deserialized messages */
            /* which are ready to be handled by the worker thread */
            std::queue<void*> m_message_queue;

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

            /* Returns reference to the inbound message buffer */
            MessageBuffer& getInboundMessageBuffer();

            /* Returns reference to the outbound message buffer */
            MessageBuffer& getOutboundMessageBuffer();

            /* Returns message queue reference */
            std::queue<void*>* getMessageQueue();

        };

    }
}

#endif
