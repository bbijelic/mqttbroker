#include<exception>

#include "ZeroBytesReceivedException.h"
        
const char * IO::ZeroBytesReceivedException::what () const throw () {
    return "Zero bytes received from the connection";
}
   