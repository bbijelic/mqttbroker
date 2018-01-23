#ifndef IO_ZERBYTESRECEIVEDEXCEPTION_H
#define IO_ZERBYTESRECEIVEDEXCEPTION_H

#include <exception>

namespace IO {

/**
 * Zero bytes received exception definition
 */
class ZeroBytesReceivedException : public std::exception {
public:
    
    virtual char const* what() const throw();
};

}

#endif