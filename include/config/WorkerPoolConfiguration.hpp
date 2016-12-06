/*
 * WorkerPoolConfiguration.hpp
 *
 *  Created on: Dec 6, 2016
 *      Author: dev
 */

#ifndef CONFIG_WORKERPOOLCONFIGURATION_HPP_
#define CONFIG_WORKERPOOLCONFIGURATION_HPP_

namespace Configuration {

/**
 * Worker pool configuration
 */
class WorkerPoolConfiguration {
private:

	/**
	 * Minimum worker pool size
	 */
	int m_min_size;

	/**
	 * Maximum worker pool size
	 */
	int m_max_size;

public:

	/**
	 * Worker pool configuration
	 */
	WorkerPoolConfiguration(int min_size, int max_size) :
			m_min_size(min_size), m_max_size(max_size) {};

	/**
	 * Destructor
	 */
	~WorkerPoolConfiguration();

	/**
	 * Returns min pool size
	 */
	int getMinPoolSize();

	/**
	 * Returns max pool size
	 */
	int getMaxPoolSize();

};

}

#endif /* CONFIG_WORKERPOOLCONFIGURATION_HPP_ */
