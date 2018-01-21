#include <iostream>

#include "easylogging++.hpp"
#include "ConnackControlPacketProducer.h"

using namespace std;
using namespace MQTT::Control::Connack;

char* ConnackControlPacketProducer::serialize(ConnackControlPacket* connack_control_packet){
    
    // Initialize connack bytes
    char* connack_bytes = new char[4];
    
    // Fixed header -> control packet type
    connack_bytes[0] = connack_control_packet->fixed_header->control_packet_type << 4;
    
    // Fixed header -> remaining length
    connack_bytes[1] = connack_control_packet->fixed_header->remaining_length;
    
    // Variable header -> acknowledge flags
    connack_bytes[2] = connack_control_packet->variable_header->acknowledge_flags->session_present;
    
    // Variable header -> connect return code
    connack_bytes[3] = connack_control_packet->variable_header->connect_return_code;
    
    // Return connack bytes
    return connack_bytes;
}