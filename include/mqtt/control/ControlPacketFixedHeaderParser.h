#ifndef MQTT_CONTROL_CONTROLPACKETFIXEDHEADERPARSER_H
#define MQTT_CONTROL_CONTROLPACKETFIXEDHEADERPARSER_H

#include <Connection.hpp>
#include "ControlPacketFixedHeader.h"

namespace MQTT {
namespace Control {

/**
 * Control packet fixed header parser
 */
class ControlPacketFixedHeaderParser {
private:

    /**
	 * Returns control byte which consists of control type field and control flags field
	 * [0..3] - Control type bit field
	 * [4..7] - Control flags bit field
	 */
	static const char getControlByte(Networking::Connection*);
    
    /**
	 * Returns message length
	 */
	static const short getRemainingLength(Networking::Connection*);
			
public:

	/**
	 * Parse message and return control packet fixed header struct
	 */
	static ControlPacketFixedHeader* parse(Networking::Connection*);
};

}}

#endif