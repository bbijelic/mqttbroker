#ifndef MQTT_CONTROL_PACKET_FIXEDHEADER_HPP
#define MQTT_CONTROL_PACKET_FIXEDHEADER_HPP

#include "ControlPacketType.h"

using namespace std;

namespace MQTT {
namespace Control {
    
/**
 * Control packet fixed header
 */
struct ControlPacketFixedHeader {
    
    // Control packet type
    ControlPacketType control_packet_type;
    
    // Control packet flags
    unsigned short control_packet_flags;
    
    // Remaining length
    unsigned short remaining_length;
    
};
    
}}

#endif