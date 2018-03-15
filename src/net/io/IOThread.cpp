/*
 * The MIT License
 *
 * Copyright 2018 bbijelic.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "net/io/IOThread.h"
#include "logging/easylogging++.h"
#include "events/Epoll.h"
#include "events/EpollException.h"
#include "net/Connection.h"
#include "mqtt/RemainingLength.h"
#include "mqtt/RemainingLengthException.h"

#include <memory>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

/* Default size: 4096 */
#define IO_READ_BUFFER_SIZE 4096

/**
 * Destructor
 */
Broker::Net::IO::IOThread::~IOThread() {
    LOG(INFO) << "Destructing IO thread";
}

/* Handles client closed connection */
void Broker::Net::IO::IOThread::handleClientClosedConnection(
        Broker::Net::Connection* connection) {

    /* Client has closed the connection */
    LOG(DEBUG) << "Client " << connection->getPeerIp() << " has closed the connection";

    /* Remove descriptor from the epoll interest list */
    m_conn_epoll->remove(connection->getDescriptor());

    /* Remove connection from heap */
    /* Destructor attempts to close the connection */
    delete connection;
}

/* Handles errors on the socket */
void Broker::Net::IO::IOThread::handleSocketError(
        Broker::Net::Connection* connection,
        epoll_event &event) {

    if (errno == EAGAIN || errno == EWOULDBLOCK) {

        /* Happens when there is no bytes in the kernels
         * read buffer ready to be read */

        LOG(DEBUG) << "Nothing to read (EAGAIN)";

        /* Re-arm the socket with exactly same events */
        m_conn_epoll->modify(connection->getDescriptor(), event);

    } else {

        /* TODO Handle other errors */

        LOG(ERROR) << "Socket error: " << errno;

        /* Remove descriptor from the epoll interest list */
        m_conn_epoll->remove(connection->getDescriptor());

        /* Remove connection from the heap */
        /* Destructor attempts to close the connection */
        delete connection;

    }

}

void Broker::Net::IO::IOThread::handleCompleteMessage(
        Broker::Net::Connection* connection,
        Broker::Net::MessageBuffer* message_buffer) {

    LOG(DEBUG) << "Handling complete message (" << message_buffer->getMessageSize()
            << " bytes) from the client " << connection->getPeerIp();

    /* TODO Deserialize MQTT message bytes from the message buffer */
    /* TODO Erase bytes from the buffer */

    /* TODO Add deserialized message to the worker queue */
    connection->getMessageQueue()->push(nullptr);

    LOG(DEBUG) << "Message queue size: " << connection->getMessageQueue()->size();
}

void Broker::Net::IO::IOThread::handleInboundBytes(
        Broker::Net::Connection* connection,
        epoll_event &event) {

    /* Socket ready for the read */
    LOG(DEBUG) << "Reading bytes from client " << connection->getPeerIp();

    /* Initialize read buffer with 8KB length */
    char read_buffer[IO_READ_BUFFER_SIZE];

    /* Try to receive enough data to fill the whole read buffer */
    int receive_result = 0;

    /* Ordinal number of the chunk */
    unsigned short chunk = 1;

    // Read until there is data to be read in the kernel input buffer
    while ((receive_result = connection->receive(read_buffer, IO_READ_BUFFER_SIZE)) > 0) {

        /* N bytes have been read from the kernel read buffer */

        LOG(DEBUG) << "Received chunk " << chunk++ << ": " << receive_result
                << " bytes from the client " << connection->getPeerIp();

        /* TODO handle incomming byte stream */
        /* In Edge-triggered mode; all available bytes have to
         * be read, since kernel will trigger an event only in
         * case when new bytes are stored into the read buffer */

        /* Get inbound message buffer of the connection */
        Broker::Net::MessageBuffer& inbound_message_buffer
                = connection->getInboundMessageBuffer();

        /* Add all received bytes to the inbound message buffer */
        inbound_message_buffer.addToBuffer(read_buffer, receive_result);

        /* Do we know already the size of the message? */
        if (inbound_message_buffer.getMessageSize() == 0) {

            /* Size of the inbound message is unknown */

            try {

                /* Try to decode remaining length from the bytes we already have */
                const unsigned int remaining_length
                        = Broker::Mqtt::RemainingLength::decode(inbound_message_buffer.getCharArray(), 1);

                /* Find how many bytes was used to encode remaining length */
                const unsigned int remaining_length_bytes
                        = Broker::Mqtt::RemainingLength::remainingLengthBytesLength(remaining_length);

                /* Calculate total message size */
                /* Control packet type byte + number of remaining length bytes + (variable header and payload) */
                const unsigned int total_message_size =
                        1 + remaining_length_bytes + remaining_length;

                LOG(DEBUG) << "Message remaining length successfully decoded: " << remaining_length << " bytes";

                /* Set message size on the inbound message buffer */
                inbound_message_buffer.setMessageSize(total_message_size);

            } catch (const Broker::Mqtt::RemainingLengthException& rmle) {
                LOG(DEBUG) << "Remaining length exception: " << rmle.what();

                /* Dont need special handling, just wait for
                 * the next bytes from the kernel buffer */
            }

        }

        if (inbound_message_buffer.getMessageSize()
                <= inbound_message_buffer.getBufferSize()) {

            /* Whole message is in inbound message buffer */
            /* It is possible that inbound message buffer contains
             * bytes from the next message also */
            handleCompleteMessage(connection, &inbound_message_buffer);

        } else if (inbound_message_buffer.getMessageSize()
                > inbound_message_buffer.getBufferSize()) {

            /* Calculate missing bytes */
            unsigned int missing_bytes
                    = inbound_message_buffer.getMessageSize() - inbound_message_buffer.getBufferSize();

            /* Still dont have whole message */
            LOG(DEBUG) << "Message not yet complete, still missing "
                    << missing_bytes << " bytes";

        }

    }

    /* Re-arm the socket to receave new events */
    m_conn_epoll->modify(connection->getDescriptor(), event);

    if (receive_result == 0) {

        /* When recv returns 0 it means end of file,
         * e.g. client closed the connection */
        /* Handle the closed connection */
        handleClientClosedConnection(connection);

    } else if (receive_result == -1) {

        /* Error has occurred on read */
        /* Handle socket errors */
        handleSocketError(connection, event);

    }

}

void Broker::Net::IO::IOThread::handleOutboundBytes(
        Broker::Net::Connection* connection,
        epoll_event& event) {

    /* Socket is ready for write operation */
    /* Generaly, socket is always write-ready, but only in case when
     * kernels write buffer is full, send() would return -1 and
     * set errno to EAGAIN or EWOULDBLOCK; in this case, socket should
     * be re-armed with EPOLLOUT event to be notified when write buffer
     * becomes ready for write again */

    LOG(DEBUG) << "Resuming sending bytes to client " << connection->getPeerIp();

    /* TODO Should to implement connection write buffer */

}

void Broker::Net::IO::IOThread::handleEventOnConnection(
        Broker::Net::Connection* connection,
        epoll_event& event) {

    if ((event.events & EPOLLERR)
            || (event.events & EPOLLHUP)
            || (event.events & EPOLLRDHUP)) {

        /* Error occurred on the socket */
        handleSocketError(connection, event);

    } else if (event.events & EPOLLIN) {

        /* Socket is ready to be read */
        /* Handle inbound byte stream */
        handleInboundBytes(connection, event);

    } else if (event.events & EPOLLOUT) {

        /* Socket is ready to be written */
        /* Handle outbound byte stream */
        handleOutboundBytes(connection, event);

    }

}

/**
 * Thread run method
 */
void* Broker::Net::IO::IOThread::run() {

    /* Set the thread name */
    /* Thread name has not other purpose other than logging */
    std::string thread_name = std::string(IO_THREAD_NAME_PREFIX);
    thread_name += std::to_string(m_tid);
    el::Helpers::setThreadName(thread_name.c_str());

    LOG(DEBUG) << "Started IO thread '" << thread_name << "'";
    LOG(DEBUG) << "Read buffer size: " << IO_READ_BUFFER_SIZE << " bytes";

    // Allocate memory for the epoll event struct array
    struct epoll_event *events = (epoll_event *) calloc(
            m_conn_epoll->getMaxEvents(), sizeof (struct epoll_event));

    /* Loop untile gracefully stoped */
    while (!m_graceful_stop) {

        // Get epoll events
        int epoll_wait_result = epoll_wait(
                m_conn_epoll->getDescriptor(),
                events,
                m_conn_epoll->getMaxEvents(), -1);

        if (epoll_wait_result == -1) {

            /* Error occured on epoll wait call */

            switch (errno) {
                case EBADF: LOG(ERROR) << EPOLL_EBADF_MSG;
                case EFAULT: LOG(ERROR) << EPOLL_EFAULT_MSG;
                case EINTR: LOG(ERROR) << EPOLL_EINTR_MSG;
                case EINVAL: LOG(ERROR) << EPOLL_EINVAL_MSG;
            }

            /* Break the while loop */
            break;
        }

        LOG(DEBUG) << "Epoll '" << m_conn_epoll->getName()
                << "' provided " << epoll_wait_result << " events to handle";

        for (int i = 0; i < epoll_wait_result; i++) {

            /* Cast the data pointer to the connection pointer */
            Broker::Net::Connection* connection
                    = (Broker::Net::Connection*) events[i].data.ptr;

            try {

                /* Handle event on a connection */
                handleEventOnConnection(connection, events[i]);

            } catch (const Broker::Events::EpollException &ee) {
                LOG(ERROR) << "Exception has occured while handling an event from the client: " << ee.what();

                /* Gracefully stop the thread */
                m_graceful_stop = true;

                /* Remove connection from the heap */
                /* Destructor attempts to close the connection */
                delete connection;
            }

        }
    }

    /* delete events struct from the heap */
    delete events;

}
