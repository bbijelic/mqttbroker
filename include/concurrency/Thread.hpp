/*
 * Thread.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <pthread.h>

namespace Concurrency {

/**
 * Thread class
 */
class Thread {
protected:

	/**
	 * Thread id
	 */
	pthread_t m_tid;

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

}

#endif /* THREAD_HPP_ */
