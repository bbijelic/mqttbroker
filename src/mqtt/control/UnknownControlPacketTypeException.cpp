#include<exception>

#include "UnknownControlPacketTypeException.h"
  
const char * MQTT::Control::UnknownControlPacketTypeException::what() const throw () {
    return "Unknown control packet type received";
}

