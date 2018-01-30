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
 * File:   TcpSocket.h
 * Author: bbijelic
 *
 * Created on January 30, 2018, 6:28 PM
 */

#ifndef NET_TCP_TCPSOCKET_H
#define NET_TCP_TCPSOCKET_H

#include "sys/Descriptor.h"

namespace Broker {
    namespace Net {
        namespace TCP {
        
            class TcpSocket : public Broker::SYS::Descriptor {
                                
            public:
                
                /* Constructor */
                TcpSocket(const int socketd_d) : Broker::SYS::Descriptor(socketd_d) {};
                
                /* Destructor */
                ~TcpSocket();
                                
            };
            
        }
    }
}

#endif /* NET_TCP_TCPSOCKET_H */

