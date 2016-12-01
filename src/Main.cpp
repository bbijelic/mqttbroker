#include "TcpListener.hpp"
#include "TcpConnectionQueue.hpp"
#include "TcpConnectionHandler.hpp"
#include "IOThread.hpp"
#include "easylogging++.hpp"

#include <sys/epoll.h>
#include <csignal>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace TCP;

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

/**
 * Main
 */
int main(int argc, char *argv[]) {

	// Load configuration from file
	el::Configurations conf("config/logger.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	// Registering shutdown handler
	registerShutdownHandler();

	// initialize epoll
	int epoll_fd = initialize_epoll();

	// Initialize IO thread
	IOThread* io_thread_1 = new IOThread(epoll_fd);
	// Start IO thread
	io_thread_1->start();

	// TCP Connection queue
	TcpConnectionQueue<TcpConnection*> tcp_connection_queue;

	// TCP connection handler
	TcpConnectionHandler* tcp_connection_handler = new TcpConnectionHandler(
			tcp_connection_queue, epoll_fd);
	// Start handler
	tcp_connection_handler->start();

	// Tcp Listener
	TcpListener* tcp_listener = new TcpListener(1883, tcp_connection_queue);
	// Start listening for incomming connections
	tcp_listener->startListening();

	// Delete tcp listener
	delete tcp_listener;

	// Delete tcp connection handler
	delete tcp_connection_handler;

	// Delete IO threads
	delete io_thread_1;

	return 0;

}

