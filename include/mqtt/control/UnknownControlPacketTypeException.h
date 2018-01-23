#ifndef MQTT_CONTROL_UNKNOWNCONTROLTYPEEXCEPTION_H
#define MQTT_CONTROL_UNKNOWNCONTROLTYPEEXCEPTION_H

#include <exception>

namespace MQTT {
namespace Control {

/**
 * Unknown control packet type exception
 */
class UnknownControlPacketTypeException : public std::exception {
public:
    
    virtual char const* what() const throw();
};
    
}}

#endif