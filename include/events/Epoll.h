#ifndef EVENTS_EPOLL_H
#define EVENTS_EPOLL_H

#include "sys/Descriptor.h"
#include "net/Connection.h"

#include <sys/epoll.h>
#include <string>

namespace Broker {
    namespace Events {

        class Epoll : public Broker::SYS::Descriptor {
        private:

            /* Epoll instance name */
            std::string m_epoll_name;
            
            /* Maximum number of events epoll_wait should return */
            unsigned int m_max_events = 64;

        public:

            /**
             * Constructor
             * @param the name of the epoll instance
             */
            Epoll(std::string);

            /* Destructor */
            ~Epoll();
            
            /**
             * Sets the max number of events for the epoll_wait
             * @param the max number of events
             */
            void setMaxEvents(unsigned int);
            
            /**
             * Returns the max number of events for the epoll_wait
             * @return the max number of events
             */
            const unsigned int getMaxEvents();

            /**
             * Returns name of the instance
             * @return the name of the epoll instance
             */
            std::string getName();

            /**
             * Adds file or socket descriptor to the interest list of epoll
             * @param the events which are of interest for the given descriptor
             * @param the pointer to the event data
             */
            void add(unsigned int, Broker::SYS::Descriptor*);
            
            /**
             * Adds file or socket descriptor to the interest list of epoll
             * @param the events which are of interest for the given descriptor
             * @param the pointer to the event data
             */
            void add(unsigned int, Broker::Net::Connection*);
                        
            /**
             * Modifies file or socket descriptor on the interest list of epoll
             * @param the socket or file descriptor
             * @param the event
             */
            void modify(int, epoll_event&);
            
            /**
             * Removes file or socket descriptor from the interes list of epoll
             * @param  the socket or file descriptor
             */
            void remove(int);
        };
    }
}

#endif