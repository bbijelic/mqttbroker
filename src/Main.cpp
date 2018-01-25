#include "net/tcp/TcpConnector.h"
#include "net/ConnectorException.h"
#include "config/ServerConfiguration.h"
#include "config/ConfigurationException.h"
#include "logging/easylogging++.h"
#include "events/Epoll.h"

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <string>

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
    Broker::Config::ServerConfiguration* server_config = loadConfiguration("config/broker.cfg");
    LOG(INFO) << "Starting broker node " << server_config->getNodeName();

    // Initialize epoll
    Broker::Events::Epoll* socket_epoll = new Broker::Events::Epoll("socket-epoll");

    // Initialize TCP connector
    Broker::Net::TCP::TcpConnector* tcp_connector = NULL;

    try {
        // Initialize TCP connector
        tcp_connector = new Broker::Net::TCP::TcpConnector(1883, std::string("0.0.0.0"), socket_epoll);

    } catch (Broker::Net::ConnectorException &conne) {
        LOG(ERROR) << "TCP connector exception: " << conne.what();
    }

    // Delete tcp connector
    if (tcp_connector) delete tcp_connector;

    // Delete socket epoll
    delete socket_epoll;

    return 0;

}

