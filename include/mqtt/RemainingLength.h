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
 * File:   RemainingLength.h
 * Author: bbijelic
 *
 * Created on March 5, 2018, 12:48 AM
 */

#ifndef REMAININGLENGTH_H
#define REMAININGLENGTH_H

#include <vector>

#define REMAINING_LENGTH_MULTIPLIER_MAX 128*128*128

namespace Broker {
    namespace Mqtt {
    
        /* Remaining length util class */
        class RemainingLength {
        public:
            
            /**
             * Decodes remaining length bytes to unsigned integer
             * 
             * @param buffer    the byte buffer
             * @param offset    the offset of the remaining length
             * 
             * @return the decoded remaining length value
             */
            static unsigned int decode(char* buffer, const unsigned int offset);
            
            /**
             * Encodes unsigned integer size to the vector of chars
             * 
             * @param size  the remaining length value
             * @return the vector containing encoded bytes
             */
            static const std::vector<char> encode(unsigned int size);
            
            /**
             * Returns how many bytes is used to encode remaining lenght info
             * 
             * @param remaining_length the remaining length value
             * 
             * @return the number of bytes used to encode remaining length;
             *  varies between 1 and 4 (inclusive)
             */
            static unsigned int remainingLengthBytesLength(unsigned int remaining_length);
            
        };
    }
}

#endif /* REMAININGLENGTH_H */

