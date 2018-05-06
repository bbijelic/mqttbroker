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

#include "net/MessageBuffer.h"
#include "logging/easylogging++.h"

/* Constructor */
Broker::Net::MessageBuffer::MessageBuffer(){

    // Set total message size
    m_total_message_size = 0;

    // Reserve vector size
    m_buffer_vector.reserve(8192);

    LOG(DEBUG) << "Initialized message buffer on the connection";
}

/* Destructor */
Broker::Net::MessageBuffer::~MessageBuffer() {
    /* Clear the message buffer */
    m_buffer_vector.clear();

    LOG(DEBUG) << "De-initialized message buffer on the connection";
}

/* Adds bytes to the buffer */
void Broker::Net::MessageBuffer::addToBuffer(char* bytes, int bytes_length){
    LOG(DEBUG) << "Adding " << bytes_length << " bytes to the inbound message buffer";

    m_buffer_vector.reserve(bytes_length);
    LOG(DEBUG) << "Reserving " << bytes_length << " bytes in the inbound message buffer";

    m_buffer_vector.insert(m_buffer_vector.end(), bytes, bytes + bytes_length);
    LOG(DEBUG) << "Inserted " << bytes_length << " bytes into the inbound message buffer";
    LOG(DEBUG) << "Size of message buffer: " << m_buffer_vector.size() << " bytes";
}

/* Returns buffer size */
unsigned int Broker::Net::MessageBuffer::getBufferSize() {
    return m_buffer_vector.size();
}

unsigned int Broker::Net::MessageBuffer::getMessageSize(){
    return m_total_message_size;
}

/* Sets total message size */
void Broker::Net::MessageBuffer::setMessageSize(unsigned int size){
    m_total_message_size = size;
}

/* Return array pointer to underlaying char array */
char* Broker::Net::MessageBuffer::getCharArray(){
    return m_buffer_vector.data();
}
