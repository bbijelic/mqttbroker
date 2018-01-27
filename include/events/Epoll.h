#ifndef EVENTS_EPOLL_H
#define EVENTS_EPOLL_H

#include <string>

namespace Broker {
    namespace Events {

        class Epoll {
        private:

            /* Epoll file descriptor */
            int m_epoll_fd;

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
             * Returns epoll file descriptor
             * @return the epoll file descriptor
             */
            const int getDescriptor();
            
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
             * @param the socket or file descriptor
             * @param the events which are of interest for the given descriptor
             */
            void addDescriptor(int, unsigned int);
            
            /**
             * Modifies file or socket descriptor on the interest list of epoll
             * @param the socket or file descriptor
             * @param the events which are of interest for the given descriptor
             */
            void modify(int, unsigned int);
        };
    }
}

#endif