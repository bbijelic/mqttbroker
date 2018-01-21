#ifndef MQTT_CONTROL_CONNACK_CONTROLPACKET_H
#define MQTT_CONTROL_CONNACK_CONTROLPACKET_H

#include "ControlPacketFixedHeader.h"

using namespace std;
using namespace MQTT::Control;

namespace MQTT {
namespace Control {
namespace Connack {

/**
 * Connect return code
 * 
 */
enum ConnectReturnCode {
    
    /**
     * Connection Accepted
     */
    CONNECTION_ACCEPTED = 0,
    
    /**
     * Connection Refused, unacceptable protocol version
     * The Server does not support the level of the MQTT protocol requested by the Client
     */
    CONNECTION_REFUSED_UNACCEPTABLE_PROTOCOL_VERSION = 1,

    /**
     * Connection Refused, identifier rejected
     * The Client identifier is correct UTF-8 but not allowed by the Server
     */
    CONNECTION_REFUSED_IDENTIFIER_REJECTED = 2,

    /**
     * Connection Refused, Server unavailable
     * The Network Connection has been made but the MQTT service is unavailable
     */
    CONNECTION_REFUSED_SERVER_UNAVAILABLE = 3,

    /**
     * Connection Refused, bad user name or password
     * The data in the user name or password is malformed
     */
    CONNECTION_REFUSED_BAD_USERNAME_OR_PASSWORD = 4,
    
    /**
     * Connection Refused, not authorized
     * The Client is not authorized to connect
     */
    CONNECTION_REFUSED_NOT_AUTHORIZED = 5

};

/**
 * Connect acknowledge flags
 */
struct ConnectAcknowledgeFlags {
    
    // Session present flag
    bool session_present;
  
};

/**
 * CONNACK variable header
 */
struct ConnackVariableHeader {

    // Connect acknowledge flags    
    ConnectAcknowledgeFlags* acknowledge_flags;
    
    // Connect return code
    ConnectReturnCode connect_return_code;
  
};
    
/**
 * CONNACK control packet
 */
struct ConnackControlPacket {
    
    // Fixed header
    ControlPacketFixedHeader* fixed_header;
    
    // Variable header
    ConnackVariableHeader* variable_header;
    
    // CONNACT has no payload
};
    
}}}

#endif