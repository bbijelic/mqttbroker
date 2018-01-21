#ifndef MQTT_CONTROL_CONNACK_CONTROLPACKET_PRODUCER_H
#define MQTT_CONTROL_CONNACK_CONTROLPACKET_PRODUCER_H

#include "ConnackControlPacket.h"

using namespace std;

namespace MQTT {
namespace Control {
namespace Connack {

class ConnackControlPacketProducer {
private:

public:

	/**
	 * Produce byte array from a given connack control packet struct
	 */
	static char* serialize(ConnackControlPacket* connack_control_packet);
};    

}}}

#endif