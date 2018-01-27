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

#include "net/ConnectionAcceptorThread.h"
#include "events/Epoll.h"
#include "logging/easylogging++.h"
#include "events/EpollException.h"

#include <memory>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

Broker::Net::ConnectionAcceptorThread::~ConnectionAcceptorThread() {

}

void* Broker::Net::ConnectionAcceptorThread::run() {
    std::string thread_name = std::string("acceptor-thread-");
    thread_name += std::to_string(m_tid);
    el::Helpers::setThreadName(thread_name.c_str());
    LOG(DEBUG) << "Running connection acceptor thread " << m_tid;

    // Allocate memory for the epoll event struct array
    struct epoll_event *events = (epoll_event *) calloc(
            m_socket_epoll->getMaxEvents(), sizeof (struct epoll_event));

    bool gracefully_stop = false;
    while (!gracefully_stop) {

        // Get epoll events
        int epoll_wait_result = epoll_wait(m_socket_epoll->getDescriptor(), events, m_socket_epoll->getMaxEvents(), -1);

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

        LOG(INFO) << "Epoll provided " << epoll_wait_result << " events to handle";
        for (int i = 0; i < epoll_wait_result; i++) {

            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                    || (events[i].events & EPOLLRDHUP)) {

                /* Error occurred */
                int error = 0;
                socklen_t errlen = sizeof (error);
                if (getsockopt(events[i].data.fd, SOL_SOCKET, SO_ERROR, (void *) &error, &errlen) == 0) {
                    LOG(DEBUG) << "Socket error: " << strerror(error);
                }


            } else if (events[i].events & EPOLLIN) {

                /* New connections */

                struct sockaddr_in address;
                socklen_t len = sizeof (address);

                memset(&address, 0, len);
                int accept_result = ::accept(events[i].data.fd, (struct sockaddr*) &address, &len);
                if (accept_result == -1) {
                    /* Error occured on accept */
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        LOG(DEBUG) << "No pending connections waiting for accept are present in the queue";
                    }
                } else {
                    
                    /* Add accepted connection to the connection 
                     * epoll instance interest list */
                    m_conn_epoll->addDescriptor(accept_result, EPOLLIN | EPOLLET | EPOLLONESHOT);
                    
                    LOG(INFO) << "Accepted connection from socket " << accept_result;
                }
            }

            try {
                
                /* Re-arm the socket */
                m_socket_epoll->modify(events[i].data.fd, EPOLLIN | EPOLLET | EPOLLONESHOT);
                
            } catch (Broker::Events::EpollException &ee){
                LOG(ERROR) << ee.what();
                
                /* Gracefully stop the thread */
                gracefully_stop = true;
            }
        }
    }

    return NULL;
}

