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

#include "net/io/ConnectionReaderThread.h"
#include "logging/easylogging++.h"
#include "events/Epoll.h"
#include "events/EpollException.h"
#include "net/Connection.h"
#include "mqtt/RemainingLength.h"

#include <memory>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IO_READ_BUFFER_SIZE 8192

/**
 * Destructor
 */
Broker::Net::IO::ConnectionReaderThread::~ConnectionReaderThread() {
    LOG(INFO) << "Destructing IO thread";
}

/* Handles client closed connection */
void Broker::Net::IO::ConnectionReaderThread::handleClientClosedConnection(
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
void Broker::Net::IO::ConnectionReaderThread::handleSocketError(
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

/**
 * Thread run method
 */
void* Broker::Net::IO::ConnectionReaderThread::run() {

    /* Set the thread name */
    /* Thread name has not other purpose other than logging */
    std::string thread_name = std::string(IO_THREAD_NAME_PREFIX);
    thread_name += std::to_string(m_tid);
    el::Helpers::setThreadName(thread_name.c_str());

    LOG(DEBUG) << "Started IO thread '" << thread_name << "'";

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

        LOG(DEBUG) << "Epoll '" << m_conn_epoll->getName() << "' provided " << epoll_wait_result << " events to handle";
        for (int i = 0; i < epoll_wait_result; i++) {

            /* Cast the data pointer to the connection pointer */
            Broker::Net::Connection* connection
                    = (Broker::Net::Connection*) events[i].data.ptr;

            try {

                if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                        || (events[i].events & EPOLLRDHUP)) {

                    /* Error occurred */
                    int error = 0;
                    socklen_t errlen = sizeof (error);
                    if (getsockopt(connection->getDescriptor(), SOL_SOCKET, SO_ERROR, (void *) &error, &errlen) == 0) {
                        LOG(ERROR) << "Socket error: " << strerror(error);
                    }

                } else if (events[i].events & EPOLLIN) {

                    /* Socket ready for the read */
                    LOG(DEBUG) << "Reading bytes from client " << connection->getPeerIp();

                    /* Initialize read buffer with 8KB length */
                    char read_buffer[IO_READ_BUFFER_SIZE];

                    /* Try to receive enough data to fill the whole read buffer */
                    int receive_result = connection->receive(read_buffer, IO_READ_BUFFER_SIZE);
                    if (receive_result == 0) {

                        /* Handle the closed connection */
                        handleClientClosedConnection(connection);

                    } else if (receive_result == -1) {

                        /* Error has occurred on read */
                        /* Handle socket errors */
                        handleSocketError(connection, events[i]);

                    } else {

                        /* N bytes have been read from the kernel read buffer */

                        LOG(DEBUG) << "Received " << receive_result << " bytes from the client " << connection->getPeerIp();

                        /* TODO handle incomming byte stream */
                        /* In Edge-triggered mode; all available bytes have to
                         * be read, since kernel will trigger an event only in
                         * case when new bytes are stored into the read buffer */

                        /* Get inbound message buffer of the connection */
                        Broker::Net::MessageBuffer& inbound_message_buffer 
                                = connection->getInboundMessageBuffer();
                        
                        if (inbound_message_buffer.isInFlight()) {

                            /* Message is in-flight --> only part of message is received already */

                        } else {
                            
                            LOG(DEBUG) << "Read buffer size: " << strlen(read_buffer);

                            /* No message bytes in the buffer at the moment */

                            /* Received at least 2 bytes?
                             * 1st byte = MQTT control packet type and flags 
                             * specific to the MQTT control packet type */
                            if (receive_result >= 2) {

                                /* Determine size of the message */
                                unsigned int remaining_length 
                                    = Broker::Mqtt::RemainingLength::decode(read_buffer, 1);
                                LOG(DEBUG) << "Remaining length: " << remaining_length;

                            } else {

                                /* Received only 1 byte */
                                /* Add it to the buffer,  */
                                // TODO add to buffer
                                inbound_message_buffer.addToBuffer(read_buffer, receive_result);
                                
                                /* Set message to be in-flight */
                                inbound_message_buffer.setInFlight(true);
                            }
                        }

                        /* Re-arm the socket to receave new events */
                        m_conn_epoll->modify(connection->getDescriptor(), events[i]);
                    }

                } else if (events[i].events & EPOLLOUT) {

                    /* Socket is ready for write operation */
                    /* Generaly, socket is always write-ready, but only in case when
                     * kernels write buffer is full, send() would return -1 and
                     * set errno to EAGAIN or EWOULDBLOCK; in this case, socket should
                     * be re-armed with EPOLLOUT event to be notified when write buffer
                     * becomes ready for write again */

                    LOG(DEBUG) << "Resuming sending bytes to client " << connection->getPeerIp();

                    /* TODO Should to implement connection write buffer */

                }

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