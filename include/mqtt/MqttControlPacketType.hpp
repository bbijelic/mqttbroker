#ifndef MQTT_CONTROLPACKETTYPE_HPP
#define MQTT_CONTROLPACKETTYPE_HPP

namespace MQTT {

enum MqttControlPacketType {

	/**
	 * Client to Server
	 * Client request to connect to Server
	 */
	CONNECT = 1,

	/**
	 * Server to Client
	 * Connect acknowledgment
	 */
	CONNACK = 2,

	/**
	 * Client to Server or Server to Client
	 * Publish message
	 */
	PUBLISH = 3,

	/**
	 * Client to Server or Server to Client
	 * Publish acknowledgment
	 */
	PUBACK = 4,

	/**
	 * Client to Server or Server to Client
	 * Publish received (assured delivery part 1)
	 */
	PUBREC = 5,

	/**
	 * Client to Server or Server to Client
	 * Publish release (assured delivery part 2)
	 */
	PUBREL = 6,

	/**
	 * Client to Server or Server to Client
	 * Publish complete (assured delivery part 3)
	 */
	PUBCOMP = 7,

	/**
	 * Client to Server
	 * Client subscribe request
	 */
	SUBSCRIBE = 8,

	/**
	 * Server to Client
	 * Subscribe acknowledgment
	 */
	SUBACK = 9,

	/**
	 * Client to Server
	 * Unsubscribe request
	 */
	UNSUBSCRIBE = 10,

	/**
	 * Server to Client
	 * Unsubscribe acknowledgment
	 */
	UNSUBACK = 11,

	/**
	 * Client to Server
	 * PING request
	 */
	PINGREQ = 12,

	/**
	 * Server to Client
	 * PING response
	 */
	PINGRESP = 13,

	/**
	 * Client to Server
	 * Client is disconnecting
	 */
	DISCONNECT = 14
};

}

#endif
