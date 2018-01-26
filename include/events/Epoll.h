#ifndef EVENTS_EPOLL_H
#define EVENTS_EPOLL_H

#include <string>

namespace Broker {
    namespace Events {

        class Epoll {
        private:

            int m_epoll_fd;

           
            std::string m_epoll_name;

        public:

            Epoll(std::string);

            ~Epoll();

            const int getDescriptor();

            std::string getName();

            void addDescriptor(int, unsigned int);
        };
    }
}

#endif