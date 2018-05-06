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
 * File:   ShutdownHandler.h
 * Author: bbijelic
 *
 * Created on February 5, 2018, 10:29 PM
 */

#ifndef SYS_SHUTDOWNHANDLER_H
#define SYS_SHUTDOWNHANDLER_H

namespace Broker {
    namespace SYS {

        /* Shutdown handler */
        class ShutdownHandler {
        private:

            /* Handles abnorminal termination of a program
             * Takes SIGABRT as argument */
            static void handleAbnormalTermination(int);

            /* Handles erroneous arithmetic operation such as a divide by 
             * zero or an operation resulting in overflow
             * Takes SIGFPE as an argument */
            static void handleErroneousArithmeticOperation(int);

            /* Handles detection of an illegal instruction
             * Takes SIGILL as an argument */
            static void handleIllegalInstruction(int);

            /* Handles receipt of an interactive attention signal
             * Takes SIGINT as an argument */
            static void handleReceiptOfInteractiveAttentionSignal(int);

            /* Handles an invalid access to storage
             * Takes SIGSEGV as an argument */
            static void handleInvalidAccessToStorage(int);

            /* Handles a termination request sent to the program
             * Takes SIGTERM as an argument */
            static void handleTerminationRequest(int);

        public:

            /* Constructor */
            ShutdownHandler();

            /* Destructor */
            ~ShutdownHandler();

        };
    }
}


#endif

