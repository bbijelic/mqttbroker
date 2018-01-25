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

#include "net/tcp/TcpConnector.h"
#include "net/ConnectorException.h"
#include "events/Epoll.h"
#include "events/EpollException.h"
#include "logging/easylogging++.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

Broker::Net::TCP::TcpConnector::TcpConnector(int port, std::string bind_address, Broker::Events::Epoll* socket_epoll) {

    m_bind_address = bind_address;
    m_port = port;
    m_socket_epoll = socket_epoll;
    
    // Create a TCP stream non-blocking socket
    m_socket_descriptor = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (m_socket_descriptor == -1) {
        throw Broker::Net::ConnectorException("Failed to open socket");
    }

    LOG(DEBUG) << "TCP connector non-blocking socket created";

    struct sockaddr_in address;

    // Set 0 to address
    memset(&address, 0, sizeof (address));

    address.sin_family = PF_INET;
    address.sin_port = htons(m_port);
    address.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    setsockopt(m_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    // Bind to the interface
    int result = bind(m_socket_descriptor, (struct sockaddr*) &address, sizeof (address));
    if (result != 0) {
        throw Broker::Net::ConnectorException("TCP connector bind failed");
    }

    LOG(DEBUG) << "TCP connector binded to the interface";

    result = listen(m_socket_descriptor, 5);
    if (result != 0) {
        throw Broker::Net::ConnectorException("TCP connector listen failed");
    }

    LOG(DEBUG) << "TCP connector listening on port " << m_port;
    
    try {
    
        // Retister socket to the epoll instance
        // Interested only in read ready event, edge-triggered, multithreaded
        m_socket_epoll->addDescriptor(m_socket_descriptor, EPOLLIN | EPOLLET | EPOLLONESHOT);
   
    } catch (Broker::Events::EpollException &ee){
        throw Broker::Net::ConnectorException(ee.what());
    }
}

Broker::Net::TCP::TcpConnector::~TcpConnector() {
    LOG(DEBUG) << "Closing TCP connector socket";
    if (m_socket_descriptor) {
        close(m_socket_descriptor);
    }
}