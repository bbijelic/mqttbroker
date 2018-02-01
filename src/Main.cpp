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
 * File:   Main.cpp
 * Author: bbijelic
 *
 * Created on January 30, 2018, 5:58 PM
 */

#include "net/io/ConnectionReaderThread.h"
#include "net/tcp/TcpConnector.h"
#include "net/ConnectionAcceptorThread.h"
#include "net/ConnectorException.h"
#include "config/ServerConfiguration.h"
#include "config/ConfigurationException.h"
#include "logging/easylogging++.h"
#include "events/Epoll.h"

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <string>
#include <memory>

INITIALIZE_EASYLOGGINGPP

void shutdownHandler(int signum) {
    LOG(INFO) << "Interrupt signal (" << signum << ") received.";
    LOG(INFO) << "Handling server shutdown...";
    exit(signum);
}

static void registerShutdownHandler() {
    LOG(INFO) << "Registering shutdown handler";

    // Register signal SIGINT and signal handler
    // Abnormal termination of the program, such as a call to abort
    signal(SIGABRT, shutdownHandler);
    // An erroneous arithmetic operation, such as a divide by zero or an operation resulting in overflow.
    signal(SIGFPE, shutdownHandler);
    // Detection of an illegal instruction
    signal(SIGILL, shutdownHandler);
    // Receipt of an interactive attention signal.
    signal(SIGINT, shutdownHandler);
    // An invalid access to storage.
    signal(SIGSEGV, shutdownHandler);
    // A termination request sent to the program.
    signal(SIGTERM, shutdownHandler);
}

static Broker::Config::ServerConfiguration* loadConfiguration(std::string config_filepath) {

    // Initialize configuration
    Broker::Config::ServerConfiguration* server_config = NULL;

    try {

        // Initialize and parse server configuration
        server_config = new Broker::Config::ServerConfiguration();
        server_config->parseConfiguration(config_filepath);

    } catch (Broker::Config::ConfigurationException &ce) {
        LOG(ERROR) << "Configuration error: " << ce.getMessage();
        abort();
    }

    return server_config;

}

/**
 * Main
 */
int main(int argc, char *argv[]) {

    // Load configuration from file
    el::Configurations conf("config/logger.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    el::Helpers::setThreadName("main");

    // Registering shutdown handler
    registerShutdownHandler();

    // Load configuration
    std::unique_ptr<Broker::Config::ServerConfiguration> server_config_ptr(
            loadConfiguration("config/broker.cfg"));
    LOG(INFO) << "Starting broker node " << server_config_ptr->getNodeName();

    // Initialize epoll shared pointer for socket and for connection events
    std::shared_ptr<Broker::Events::Epoll> socket_epoll_ptr(
            new Broker::Events::Epoll("socket-epoll"));

    // Initialize epoll shared pointer for incomming connection events
    std::shared_ptr<Broker::Events::Epoll> conn_epoll_ptr(
            new Broker::Events::Epoll("connection-epoll"));

    try {
        
        /* IO thread smart pointer init */
        std::unique_ptr<Broker::Net::IO::ConnectionReaderThread> io_thread_ptr_1(
            new Broker::Net::IO::ConnectionReaderThread(conn_epoll_ptr));
               
        /* Start IO thread */
        io_thread_ptr_1->start();

        // Initialize TCP connector unique pointer
        std::unique_ptr<Broker::Net::TCP::TcpConnector> tcp_connector_ptr(
                new Broker::Net::TCP::TcpConnector(
                1883, std::string("0.0.0.0"), socket_epoll_ptr));

        // Start connector
        // Creates socket, binds on interface and starts to listen
        tcp_connector_ptr->start();

        // Initialize connection acceptor thread unique pointer
        std::unique_ptr<Broker::Net::ConnectionAcceptorThread> conn_acceptor_ptr_1(
                new Broker::Net::ConnectionAcceptorThread(
                socket_epoll_ptr, conn_epoll_ptr));
        
        // Start the thread
        conn_acceptor_ptr_1->start();
                
        /* Join IO threads */
        io_thread_ptr_1->join();
        
        // Join the acceptor threads
        conn_acceptor_ptr_1->join();

        // Stop the TCP connector
        tcp_connector_ptr->stop();

    } catch (Broker::Net::ConnectorException &conne) {
        LOG(ERROR) << "TCP connector exception: " << conne.what();
    }

    return 0;
}

