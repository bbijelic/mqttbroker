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

#ifndef NET_IO_CONNECTIONREADERTHREAD_H
#define NET_IO_CONNECTIONREADERTHREAD_H

#include "../../concurrent/Thread.h"
#include "../../events/Epoll.h"

#include <string>
#include <memory>

#define IO_THREAD_NAME_PREFIX "io-thread-"

namespace Broker {
    namespace Net {
        namespace IO {

            /* Connection reader thread */
            class ConnectionReaderThread : public Broker::Concurrent::Thread {
            private:

                /* Connection epoll instance
                 * All client sockets are registered to this epoll instance */
                const std::shared_ptr<Broker::Events::Epoll>& m_conn_epoll;

                /**
                 * Triggered when EPOLLIN event received on the socket, but
                 * obtained 0 bytes from the kernel read buffer via read call
                 * 
                 * @param connection    the client connection
                 */
                void handleClientClosedConnection(
                        Broker::Net::Connection* connection);

                /**
                 * Triggered when read operation on socket resulted with -1.
                 * Errno external variable is set accordingly.
                 * 
                 * @param connection    the client connection
                 * @param event         the epoll event
                 */
                void handleSocketError(
                        Broker::Net::Connection* connection,
                        epoll_event &event);

                /**
                 * Handles complete message from the client
                 * @param connection        the client connection
                 * @param message_buffer    the message buffer containing whole message
                 */
                void handleCompleteMessage(
                        Broker::Net::Connection* connection,
                        Broker::Net::MessageBuffer *message_buffer);

            public:

                /* Constructor */
                ConnectionReaderThread(
                        const std::shared_ptr<Broker::Events::Epoll>& conn_epoll)
                : m_conn_epoll(conn_epoll) {
                };

                /* Destructor */
                ~ConnectionReaderThread();

                /* Run */
                void* run();
            };

        }
    }
}


#endif

