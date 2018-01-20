#ifndef MQTT_CONTROL_CONNECT_CONTROLPACKETPARSER_H
#define MQTT_CONTROL_CONNECT_CONTROLPACKETPARSER_H

#include <string>
#include "ConnectControlPacket.h"

using namespace std;

namespace MQTT {
namespace Control {
namespace Connect {

class ConnectControlPacketParser {
private:

public:

	/**
	 * Parse message and return connect control packet struct instance
	 */
	static ConnectControlPacket* parse(char*);
};

}}}

#endif