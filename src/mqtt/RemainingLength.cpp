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

#include "mqtt/RemainingLength.h"
#include "mqtt/RemainingLengthException.h"

unsigned int Broker::Mqtt::RemainingLength::decode(char* buffer, const unsigned int offset) {
    // Initialize remaining length
    unsigned int remaining_length_value = 0;

    // Multiplier
    unsigned int multiplier = 1;

    // At the beginning next encoded byte has index equal to the offset
    unsigned short next_encoded_byte_index = offset;

    // Initialize encoded byte
    unsigned char encoded_byte = 0x00;

    do {

        if (multiplier > REMAINING_LENGTH_MULTIPLIER_MAX)
            throw Broker::Mqtt::RemainingLengthException("Malformed remaining length");

        // Obtain next encoded byte
        encoded_byte = buffer[next_encoded_byte_index];

        // Calculate remaining length value
        remaining_length_value += (encoded_byte & 127) * multiplier;
        multiplier *= 128;

        // Increment next encoded byte index
        next_encoded_byte_index++;

    } while ((encoded_byte & 128) != 0);

    // Return remainign length value
    return remaining_length_value;
}

const std::vector<char> Broker::Mqtt::RemainingLength::encode(unsigned int size) {
    
    // Initialize encoded vector
    std::vector<char> encoded_vector;
    
    // Max number of bytes in the vector is 4, reserve memory
    encoded_vector.reserve(4);
    
    do {
        
        // initialize encoded byte
        char encoded_byte = size % 128;
        size = size / 128;
        if(size > 0) encoded_byte = encoded_byte | 128;
        encoded_vector.push_back(encoded_byte);
        
    } while( size > 0 );
    
    // return encoded vector
    return encoded_vector;
}