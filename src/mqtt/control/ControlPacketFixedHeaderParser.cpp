#include <iostream>

#include "easylogging++.hpp"
#include "ControlPacketFixedHeaderParser.h"
#include "ZeroBytesReceivedException.h"
#include "UnknownControlPacketTypeException.h"
#include "MalformedRemainingLengthException.h"

/**
 * Receives first byte and returns it
 * Throws:
 *      IO::ZeroBytesReceivedException -  When bytes is received from the socket
 */
const char MQTT::Control::ControlPacketFixedHeaderParser::getControlByte(Networking::Connection* connection){
    // Read only one byte, which is message control byte
	char control_type_byte_buffer[1];
	if (connection->receive(control_type_byte_buffer, 1) == 0) {
	    // Client closed connection
		throw IO::ZeroBytesReceivedException();
	}
	
	// Return control type byte
	return control_type_byte_buffer[0];
}

const short MQTT::Control::ControlPacketFixedHeaderParser::getRemainingLength(Networking::Connection* connection){
    int multiplier = 1;
	int length = 0;
	char buffer[1];
	
	do {
	    
		// Get next byte of the stream
		int received = connection->receive(buffer, sizeof buffer);

		if (received == 0) {
			// Client closed connection
            throw IO::ZeroBytesReceivedException();
		}

		length += (buffer[0] & 127) * multiplier;
		multiplier *= 128;

		if (multiplier > 128 * 128 * 128)
            // Malformed remaining length
			throw MQTT::Control::MalformedRemainingLengthException();

	} while ((buffer[0] & 128) != 0);

	// Return length
	return length;
}

MQTT::Control::ControlPacketFixedHeader* MQTT::Control::ControlPacketFixedHeaderParser::parse(Networking::Connection* connection){
    LOG(DEBUG)<< "Parsing fixed header of a control packet";
    
    // Fixed header instance
    MQTT::Control::ControlPacketFixedHeader* fixed_header = new ControlPacketFixedHeader();
    
    // Get control type byte
    const char control_type_byte = MQTT::Control::ControlPacketFixedHeaderParser::getControlByte(connection);
    
    // Shift bytes to get control packet type
	const unsigned short control_packet_type_shift = control_type_byte >> 4;	
	// Shift bytes to get control packet flags
	fixed_header->control_packet_flags = ( (control_type_byte & 0x0F)<<4 | (control_type_byte & 0xF0)>>4 ) >> 4;
		
	// Validate parsed control packet type
	// Must be one of following
	if(control_packet_type_shift == MQTT::Control::ControlPacketType::CONNECT){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::CONNECT;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::PUBLISH){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::PUBLISH;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::PUBACK){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::PUBACK;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::PUBREC){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::PUBREC;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::PUBREL){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::PUBREL;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::PUBCOMP){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::PUBCOMP;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::SUBSCRIBE){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::SUBSCRIBE;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::UNSUBSCRIBE){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::UNSUBSCRIBE;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::PINGREQ){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::PINGREQ;
	} else if(control_packet_type_shift == MQTT::Control::ControlPacketType::DISCONNECT){
	    fixed_header->control_packet_type = MQTT::Control::ControlPacketType::DISCONNECT;
    } else {   
	       // Throw an exception
	       throw MQTT::Control::UnknownControlPacketTypeException(); 
    }
    
    // Get remaining length
    fixed_header->remaining_length = MQTT::Control::ControlPacketFixedHeaderParser::getRemainingLength(connection);
        
    // Return the connect control fixed header packet
    return fixed_header;
}
