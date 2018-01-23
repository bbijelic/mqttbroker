#include<exception>

#include "MalformedRemainingLengthException.h"
  
const char * MQTT::Control::MalformedRemainingLengthException::what() const throw () {
    return "Remaining length of fixed header is malformed";
}

