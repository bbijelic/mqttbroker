#ifndef MQTT_CONTROL_CONNECT_CONTROLPACKET_H
#define MQTT_CONTROL_CONNECT_CONTROLPACKET_H

#include <string>

using namespace std;

namespace MQTT {
namespace Control {
namespace Connect {

/**
 * CONNECT Control Packet Variable Header Connect Flags
 */
struct ConnectFlags {

    // Reserved flag
    bool reserved;
    
    // Clean session
    bool clean_session;
    
    // Will flag
    bool will_flag;
    
    // Will QoS
    unsigned short will_qos;
    
    // Will retain
    bool will_retain;
    
    // Password flag
    bool password_flag;
    
    // Username flag
    bool username_flag;
};
    
/**
 * CONNECT Control Packet Variable Header
 */
struct ConnectVariableHeader {

    // Protocol namw
    string protocol_name;
    
    // Protocol level
    unsigned protocol_level;
    
    // Connect flags
    ConnectFlags* connect_flags;
    
    // Keep Alive
    short keep_alive; 
};

/**
 * CONNECT Control Packet Payload
 */
struct ConnectPayload {

    // Client identifier
    string client_identifier;
    
    // Will topic
    string will_topic;
    
    // Will message
    char will_message[];
    
    // Username
    string username;
    
    // Password
    char password[];
};

/**
 * CONNECT Control Packet
 */
struct ConnectControlPacket {

    // Variable header
    ConnectVariableHeader* variable_header;
    
    // Payload
    ConnectPayload* payload;  
};
    
}}}

#endif