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

        // Initialize TCP connector unique pointer
        std::unique_ptr<Broker::Net::TCP::TcpConnector> tcp_connector_ptr(
                new Broker::Net::TCP::TcpConnector(
                1883, std::string("0.0.0.0"), socket_epoll_ptr));

        // Start connector
        // Creates socket, binds on interface and starts to listen
        tcp_connector_ptr->start();

        // Initialize connection acceptor thread unique pointer
        std::unique_ptr<Broker::Net::ConnectionAcceptorThread> conn_acceptor_ptr(
                new Broker::Net::ConnectionAcceptorThread(
                socket_epoll_ptr, conn_epoll_ptr));
        
        // Start the thread
        conn_acceptor_ptr->start();
        // Join the thread
        conn_acceptor_ptr->join();

        // Stop the TCP connector
        tcp_connector_ptr->stop();

    } catch (Broker::Net::ConnectorException &conne) {
        LOG(ERROR) << "TCP connector exception: " << conne.what();
    }

    return 0;
}

