#include <ConfigurationException.hpp>
#include <Connection.hpp>
#include <ConnectionHandler.hpp>
#include <ConnectionQueue.hpp>
#include <easylogging++.hpp>
#include <IOThread.hpp>
#include <ListenerManager.hpp>
#include <sys/epoll.h>
#include <ServerConfiguration.hpp>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <string>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace IO;
using namespace Networking;
using namespace Networking::TCP;
using namespace Networking::Security;
using namespace Configuration;

static int initialize_epoll() {
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		LOG(ERROR)<< "epoll_create() failed: " << errno;
		abort();
	}

	LOG(INFO)<< "epoll initialized";

	return epoll_fd;
}

void shutdownHandler(int signum) {
	LOG(INFO)<< "Interrupt signal (" << signum << ") received.";
	LOG(INFO) << "Handling server shutdown...";
	exit(signum);
}

static void registerShutdownHandler() {
	LOG(INFO)<< "Registering shutdown handler";

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

static ServerConfiguration* loadConfiguration(string config_filepath) {

	// Initialize configuration
	ServerConfiguration* server_config = NULL;

	try {

		// Initialize and parse server configuration
		server_config = new ServerConfiguration();
		server_config->parseConfiguration(config_filepath);

	} catch (ConfigurationException &ce) {
		LOG(ERROR)<< "Configuration error: " << ce.getMessage();
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

	// Registering shutdown handler
	registerShutdownHandler();

	// Load configuration
	ServerConfiguration* server_config = loadConfiguration("config/broker.cfg");

	LOG(INFO)<< "Starting broker node " << server_config->getNodeName();

	// initialize epoll
	int epoll_fd = initialize_epoll();

	// Initialize IO thread
	IOThread* io_thread = new IOThread(epoll_fd);
	// Start IO thread
	io_thread->start();

	// Connection queue
	ConnectionQueue<Connection*> connection_queue;

	// Connection handler
	ConnectionHandler* connection_handler = new ConnectionHandler(
			connection_queue, epoll_fd);
	// Start handler
	connection_handler->start();

	// Initialize listener manager
	ListenerManager* listener_mngr = new ListenerManager(connection_queue, server_config);
	// Start listeners
	listener_mngr->startListeners();

	// Join the connection handler
	connection_handler->join();
	// Join the IO Thread
	io_thread->join();

	// Delete server configuration
	delete server_config;

	// Delete listener manager
	delete listener_mngr;

	// Delete tcp connection handler
	delete connection_handler;

	// Delete IO threads
	delete io_thread;

	return 0;

}

