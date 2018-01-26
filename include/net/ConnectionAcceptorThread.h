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

/* 
 * File:   ConnectionAcceptorThread.h
 * Author: bbijelic
 *
 * Created on January 26, 2018, 10:03 PM
 */

#ifndef NET_CONNECTIONACCEPTORTHREAD_H
#define NET_CONNECTIONACCEPTORTHREAD_H

#include "../concurrent/Thread.h"
#include "../events/Epoll.h"

#include <memory>

namespace Broker {
    namespace Net {

        class ConnectionAcceptorThread : public Broker::Concurrent::Thread {
        private:

            /* Epoll shared pointer of socket epoll */
            const std::shared_ptr<Broker::Events::Epoll>& m_socket_epoll_ptr;

            /* Epoll shared pointer of connection epoll */
            const std::shared_ptr<Broker::Events::Epoll>& m_conn_epoll_ptr;

        public:

            /* Constructor */
            ConnectionAcceptorThread(
                    const std::shared_ptr<Broker::Events::Epoll>& socket_epoll_ptr,
                    const std::shared_ptr<Broker::Events::Epoll>& conn_epoll_ptr)
            : m_socket_epoll_ptr(socket_epoll_ptr), m_conn_epoll_ptr(conn_epoll_ptr) {};

            /* Destructor */
            ~ConnectionAcceptorThread();

            /* Run function */
            void* run();

        };
    }
}

#endif

