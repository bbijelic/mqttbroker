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
 * File:   Descriptor.h
 * Author: bbijelic
 *
 * Created on January 30, 2018, 6:40 PM
 */

#ifndef SYS_DESCRIPTOR_H
#define SYS_DESCRIPTOR_H

namespace Broker {
    namespace SYS {

        /* Descriptor class */
        class Descriptor {
        protected:

            /* Descriptor */
            int m_descriptor;

        public:

            /* Constructor */
            Descriptor() {};
            
            /* Constructor */
            Descriptor(const int descriptor) : m_descriptor(descriptor) {
            };
            
            void setDescriptor(const int);

            /* Descriptor getter */
            const int getDescriptor();

        };
    }
}

#endif

