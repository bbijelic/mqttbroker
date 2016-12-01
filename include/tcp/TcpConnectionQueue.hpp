/*
 * WorkQueue.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: dev
 */

#ifndef CONCURRENCY_TCPCONNECTIONQUEUE_HPP_
#define CONCURRENCY_TCPCONNECTIONQUEUE_HPP_

#include <pthread.h>
#include <list>

using namespace std;

namespace TCP {

/**
 * Template TCP connection queue class
 */
template<typename T> class TcpConnectionQueue {
private:

	/**
	 * The queue
	 */
	list<T> m_queue;

	/**
	 * Thread mutex
	 */
	pthread_mutex_t m_mutex;

	/**
	 * Thread cond
	 */
	pthread_cond_t m_condv;

public:

	/**
	 * Constructor
	 */
	TcpConnectionQueue() {
		pthread_mutex_init(&m_mutex, NULL);
		pthread_cond_init(&m_condv, NULL);
	}

	/**
	 * Destructor
	 */
	~TcpConnectionQueue() {
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_condv);
	}

	/**
	 * Add item to the queue
	 */
	void add(T item) {
		pthread_mutex_lock(&m_mutex);
		m_queue.push_back(item);
		pthread_cond_signal(&m_condv);
		pthread_mutex_unlock(&m_mutex);
	}

	/**
	 * Remove item from the work queue
	 */
	T remove() {
		pthread_mutex_lock(&m_mutex);
		while (m_queue.size() == 0) {
			pthread_cond_wait(&m_condv, &m_mutex);
		}

		T item = m_queue.front();
		m_queue.pop_front();
		pthread_mutex_unlock(&m_mutex);
		return item;
	}

	/**
	 * Get queue size
	 */
	int size() {
		pthread_mutex_lock(&m_mutex);
		int size = m_queue.size();
		pthread_mutex_unlock(&m_mutex);
		return size;
	}
};

}

#endif /* CONCURRENCY_TCPCONNECTIONQUEUE_HPP_ */
