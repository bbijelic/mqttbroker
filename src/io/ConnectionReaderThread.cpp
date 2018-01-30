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

#include "io/ConnectionReaderThread.h"
#include "logging/easylogging++.h"
#include "events/Epoll.h"
#include "events/EpollException.h"
#include "net/Connection.h"

#include <memory>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * Destructor
 */
Broker::IO::ConnectionReaderThread::~ConnectionReaderThread() {
    LOG(INFO) << "Destructing IO thread";
}

/**
 * Thread run method
 * @return 
 */
void* Broker::IO::ConnectionReaderThread::run() {

    /* Set the thread name */
    std::string thread_name = std::string("io-thread-");
    thread_name += std::to_string(m_tid);
    el::Helpers::setThreadName(thread_name.c_str());

    LOG(DEBUG) << "Started IO thread '" << thread_name << "'";

    // Allocate memory for the epoll event struct array
    struct epoll_event *events = (epoll_event *) calloc(
            m_conn_epoll->getMaxEvents(), sizeof (struct epoll_event));

    bool gracefully_stop = false;
    while (!gracefully_stop) {

        // Get epoll events
        int epoll_wait_result = epoll_wait(
                m_conn_epoll->getDescriptor(),
                events,
                m_conn_epoll->getMaxEvents(), -1);

        if (epoll_wait_result == -1) {

            /* Error occured on epoll wait call */

            if (errno == EBADF) {
                LOG(ERROR) << "Provided epoll file descriptor is not a valid file descriptor";

            } else if (errno == EFAULT) {
                LOG(ERROR) << "The memory area pointed to by events is not accessible with write permissions";

            } else if (errno == EINTR) {
                LOG(ERROR) << "The call was interrupted by a signal handler";

            } else if (errno == EINVAL) {
                LOG(ERROR) << "Provided epoll file descriptor is not a epoll file descriptor or max events is se to be zero or less";
            }

            /* Gracefully stop the thread */
            gracefully_stop = true;
            break;
        }

        LOG(INFO) << "Epoll '" <<  m_conn_epoll->getName() << "' provided " << epoll_wait_result << " events to handle";
        for (int i = 0; i < epoll_wait_result; i++) {

            /* Cast the data pointer to the connection pointer */
            Broker::Net::Connection* connection
                    = (Broker::Net::Connection*) events[i].data.ptr;

            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                    || (events[i].events & EPOLLRDHUP)) {

                /* Error occurred */
                int error = 0;
                socklen_t errlen = sizeof (error);
                if (getsockopt(connection->getDescriptor(), SOL_SOCKET, SO_ERROR, (void *) &error, &errlen) == 0) {
                    LOG(DEBUG) << "Socket error: " << strerror(error);
                }

            } else if (events[i].events & EPOLLOUT) {

                /* Socket read for the write */
                LOG(DEBUG) << "Reading bytes from client " << connection->getPeerIp();

            } else if (events[i].events & EPOLLIN) {

                /* Socket ready for the read */
                LOG(DEBUG) << "Writting bytes to client " << connection->getPeerIp();

            }

            try {

                /* Re-arm the socket */
                m_conn_epoll->modify(connection->getDescriptor(),
                        events[i]);

            } catch (const Broker::Events::EpollException &ee) {
                LOG(ERROR) << ee.what();

                /* Gracefully stop the thread */
                gracefully_stop = true;
                break;
            }
        }
    }
}