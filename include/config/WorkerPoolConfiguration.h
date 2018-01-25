#ifndef CONFIG_WORKERPOOLCONFIGURATION_H
#define CONFIG_WORKERPOOLCONFIGURATION_H

namespace Broker {
namespace Config {

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

}}

#endif