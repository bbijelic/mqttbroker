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
 * File:   ConnectionReaderThread.h
 * Author: bbijelic
 *
 * Created on January 28, 2018, 1:58 PM
 */

#ifndef IO_CONNECTIONREADERTHREAD_H
#define IO_CONNECTIONREADERTHREAD_H

#include "../concurrent/Thread.h"
#include "../events/Epoll.h"

#include <string>
#include <memory>

namespace Broker {
    namespace IO {

        /* Connection reader thread */
        class ConnectionReaderThread : public Broker::Concurrent::Thread {
        private:

            /* Connection epoll instance
             * All client sockets are registered to this epoll instance */
            const std::shared_ptr<Broker::Events::Epoll>& m_conn_epoll;

        public:

            /* Constructor */
            ConnectionReaderThread(
                    const std::shared_ptr<Broker::Events::Epoll>& conn_epoll)
            : m_conn_epoll(conn_epoll) {};

            /* Destructor */
            ~ConnectionReaderThread();

            /* Run */
            void* run();
        };

    }
}

#endif

