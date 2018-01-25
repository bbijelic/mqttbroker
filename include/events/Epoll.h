#ifndef EVENTS_EPOLL_H
#define EVENTS_EPOLL_H

#include <sys/epoll.h>
#include <string>

namespace Broker {
namespace Events {

class Epoll {
private:

	/**
	 * Epoll file descriptor
	 */
	int m_epoll_fd;   
	
	/**
	 * Epoll instance name
	 */
	std::string m_epoll_name;
    
public:

	/**
	 * Constructor
	 */
	Epoll(std::string);

	/**
	 * Destructor
	 */
	~Epoll();

	/**
	 * Returns epoll file descriptor
	 */
	const int getDescriptor();    
	
	/**
	 * Returns epoll instance name
	 */
	std::string getName();
};

}}

#endif