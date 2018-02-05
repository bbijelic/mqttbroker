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

#include "sys/ShutdownHandler.h"
#include "logging/easylogging++.h"

#include <cerrno>
#include <csignal>
#include <cstdlib>

Broker::SYS::ShutdownHandler::ShutdownHandler() {
    // Abnormal termination of the program, such as a call to abort
    signal(SIGABRT, handleAbnormalTermination);
    // An erroneous arithmetic operation, such as a divide by zero or an operation resulting in overflow.
    signal(SIGFPE, handleErroneousArithmeticOperation);
    // Detection of an illegal instruction
    signal(SIGILL, handleIllegalInstruction);
    // Receipt of an interactive attention signal.
    signal(SIGINT, handleReceiptOfInteractiveAttentionSignal);
    // An invalid access to storage.
    signal(SIGSEGV, handleInvalidAccessToStorage);
    // A termination request sent to the program.
    signal(SIGTERM, handleTerminationRequest);
}

Broker::SYS::ShutdownHandler::~ShutdownHandler() {
    
}

void Broker::SYS::ShutdownHandler::handleAbnormalTermination(int signum) {
    LOG(DEBUG) << "Handling abnormal termination of a program";
    exit(signum);
}

void Broker::SYS::ShutdownHandler::handleErroneousArithmeticOperation(int signum) {
    LOG(DEBUG) << "Handling erroneous arithmetic operation, closing";
    exit(signum);
}

void Broker::SYS::ShutdownHandler::handleIllegalInstruction(int signum) {
    LOG(DEBUG) << "Handling illegal instruction, closing";
    exit(signum);
}

void Broker::SYS::ShutdownHandler::handleInvalidAccessToStorage(int signum) {
    LOG(DEBUG) << "Handling invalid access to storage, closing";
    exit(signum);
}

void Broker::SYS::ShutdownHandler::handleReceiptOfInteractiveAttentionSignal(int signum) {
    LOG(DEBUG) << "Handling receipt of an interactive attention signal, closing";
    exit(signum);
}

void Broker::SYS::ShutdownHandler::handleTerminationRequest(int signum) {
    LOG(DEBUG) << "Handling termination request, closing";
    exit(signum);
}