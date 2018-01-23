#ifndef MQTT_CONTROL_MALFORMEDREMAININGLENGTHEXCEPTION_H
#define MQTT_CONTROL_MALFORMEDREMAININGLENGTHEXCEPTION_H

#include <exception>

namespace MQTT {
namespace Control {

/**
 * Malformed remaining length exception
 */
class MalformedRemainingLengthException : public std::exception {
public:
    
    virtual char const* what() const throw();
};
    
}}

#endif