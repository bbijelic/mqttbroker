#include "TcpListener.hpp"
#include "TcpConnectionQueue.hpp"
#include "TcpConnectionHandler.hpp"
#include "IOThread.hpp"

#include <sys/epoll.h>
#include <iostream>

using namespace std;
using namespace TCP;

static int initialize_epoll() {
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		cout << "epoll_create() failed: " << errno << endl;
		abort();
	}

	cout << "epoll initialized" << endl;

	return epoll_fd;
}

/**
 * Main
 */
int main(int argc, char *argv[]) {

	// initialize epoll
	int epoll_fd = initialize_epoll();

	// Initialize IO thread
	IOThread* io_thread_1 = new IOThread(epoll_fd);
	// Start IO thread
	io_thread_1->start();

	// Initialize IO thread
	IOThread* io_thread_2 = new IOThread(epoll_fd);
	// Start IO thread
	io_thread_2->start();

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
	delete io_thread_2;

	return 0;

}

