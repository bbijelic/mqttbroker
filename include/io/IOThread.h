#ifndef IO_IOTHREAD_H_
#define IO_IOTHREAD_H

#include "Connection.hpp"
#include "Thread.hpp"
#include "ControlPacketType.h"
#include "ZeroBytesReceivedException.h"
#include "UnknownControlPacketTypeException.h"

using namespace Concurrency;
using namespace Networking;
using namespace MQTT::Control;

namespace IO {

class IOThread : public Thread {
private:

	/**
	 * Epoll file descriptor
	 */
	int m_epoll_fd;

	/**
	 * Reenables socket descriptor
	 */
	bool reenableSocketDescriptor(int);
	
	/**
	 * Handles error on a epoll
	 */
	void onEpollError(Connection*);
	
	/**
	 * Handles write-ready event from the kernel.
	 * Flushes output buffer to the socket if anything in
	 */
	void onEpollWriteReady(Connection*);
	
	/**
	 * Handles read-ready event from the kernel
	 */
	void onEpollReadReady(Connection*);

public:

	/**
	 * Constructor
	 */
	IOThread(int epoll_fd) : m_epoll_fd(epoll_fd) {};

	/**
	 * Run
	 */
	void* run();

};

}

#endif
