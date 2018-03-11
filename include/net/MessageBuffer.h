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
 * File:   MessageBuffer.h
 * Author: bbijelic
 *
 * Created on March 4, 2018, 5:52 PM
 */

#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include <vector>

namespace Broker {
    namespace Net {
        
        /* Message buffer class */
        class MessageBuffer {
        private:
            
            /* Is in flight */
            bool m_is_inflight;
            
            /* Message buffer vector */
            std::vector<char> m_buffer_vector;
            
            /* Total message size */
            unsigned int m_total_message_size;

        public:
            
            /* Constructor */
            MessageBuffer();
            
            /* Destructor */
            ~MessageBuffer();
            
            /* Returns true when message is in-flight, 
             * i.e. only half of message is received */
            bool isInFlight();
            
            /* Sets the in-flight flag */
            void setInFlight(bool);
            
            /* Adds bytes to the buffer  */
            void addToBuffer(char* bytes, int bytes_length);
            
            /* Returns total message size in bytes */
            unsigned int getMessageSize();
            
            /* Sets total message size */
            void setMessageSize(unsigned int size);
            
        };

    }
}

#endif /* MESSAGEBUFFER_H */

