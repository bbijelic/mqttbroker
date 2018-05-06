#include "concurrent/Thread.h"

#include <stddef.h>

static void* runThread(void* arg) {
	return ((Broker::Concurrent::Thread*)arg)->run();
}

Broker::Concurrent::Thread::Thread() : m_tid(0), m_running(0), m_detached(0), m_graceful_stop(false) {}

Broker::Concurrent::Thread::~Thread() {
	if (m_running == 1 && m_detached == 0) {
		pthread_detach(m_tid);
	}
	if (m_running == 1) {
		pthread_cancel(m_tid);
	}
}

int Broker::Concurrent::Thread::start() {
	int result = pthread_create(&m_tid, NULL, runThread, this);
	if (result == 0) {
		m_running = 1;
	}

	return result;
}

void Broker::Concurrent::Thread::gracefulStop(){
	m_graceful_stop = true;
}

int Broker::Concurrent::Thread::join() {
	int result = -1;
	if (m_running == 1) {
		result = pthread_join(m_tid, NULL);
		if (result == 0) {
			m_detached = 0;
		}
	}

	return result;
}

int Broker::Concurrent::Thread::detach() {
	int result = -1;
	if (m_running == 1 && m_detached == 0) {
		result = pthread_detach(m_tid);
		if (result == 0) {
			m_detached = 1;
		}
	}

	return result;
}

pthread_t Broker::Concurrent::Thread::self() {
	return m_tid;
}