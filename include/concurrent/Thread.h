#ifndef CONCURRENT_THREAD_H
#define CONCURRENT_THREAD_H

#include <pthread.h>

namespace Broker {
namespace Concurrent {

/**
 * Thread class
 */
class Thread {
protected:

	/**
	 * Thread id
	 */
	pthread_t m_tid;

	/**
	 * Should thread gracefully stop
	 */
	bool m_graceful_stop = false;

private:

	/**
	 * Is running
	 */
	int m_running;

	/**
	 * Is detached
	 */
	int m_detached;

public:

	/**
	 * Constructor
	 */
	Thread();

	/**
	 * Destructor
	 */
	virtual ~Thread();

	/**
	 * Start thread
	 */
	int start();

	/**
	 * Graceful stop
	 */
	void gracefulStop();

	/**
	 * Join thread
	 */
	int join();

	/**
	 * Detach thread
	 */
	int detach();

	/**
	 * ???
	 */
	pthread_t self();

	/**
	 * Thread run method
	 */
	virtual void* run() =0;
};

}}

#endif