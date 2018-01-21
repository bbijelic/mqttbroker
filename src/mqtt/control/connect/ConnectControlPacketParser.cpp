#include <iostream>

#include "easylogging++.hpp"
#include "ConnectControlPacketParser.h"

using namespace std;
using namespace MQTT::Control::Connect;

ConnectControlPacket* ConnectControlPacketParser::parse(char* message){
    LOG(INFO)<< "Parsing CONNECT message";
    
    // Variable header instance
    ConnectVariableHeader* variable_header = new ConnectVariableHeader();
    
    // Protocol name
    char protocol_name[5] = {0};
    memcpy(protocol_name, message + 2, 4);
    variable_header->protocol_name = protocol_name; 
    LOG(DEBUG) << "Variable Header --> Protocol name: " << variable_header->protocol_name;
    
    // Protocol level
    variable_header->protocol_level = message[6];
    LOG(DEBUG) << "Variable Header --> Protocol level: " << variable_header->protocol_level;
        
    // Connect flags
    ConnectFlags* connect_flags = new ConnectFlags();
    char connect_flag_byte = message[7];
    
    // Reserved flag
    connect_flags->reserved = (bool)(0x01 & connect_flag_byte);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Reserved: " << connect_flags->reserved;
    
    // Clean session flag
    connect_flags->clean_session = (bool)((0x02 & connect_flag_byte) >> 1);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Clean Session: " << connect_flags->clean_session;
    
    // Will flag
    connect_flags->will_flag = (bool)((0x04 & connect_flag_byte) >> 2);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Will Flag: " << connect_flags->will_flag;
    
    // Will QoS
    connect_flags->will_qos = (unsigned short)((0x18 & connect_flag_byte) >> 3);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Will QoS: " << connect_flags->will_qos;
    
    // Will Retain
    connect_flags->will_retain = (bool)((0x20 & connect_flag_byte) >> 5);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Will Retain: " << connect_flags->will_retain;
    
    // Password flag
    connect_flags->password_flag = (bool)((0x40 & connect_flag_byte) >> 6);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Password flag: " << connect_flags->password_flag;
    
    // Username flag
    connect_flags->username_flag = (bool)((0x80 & connect_flag_byte) >> 7);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Username flag: " << connect_flags->username_flag;
    
    // Set connect flags on the variable header
    variable_header->connect_flags = connect_flags;
        
    // Protocol name
    variable_header->keep_alive = *(message+8) << 8 | *(message+9);
    LOG(DEBUG) << "Variable Header --> Keep Alive: " << variable_header->keep_alive;
    
    // Initialize connect payload
    ConnectPayload* connect_payload = new ConnectPayload();
    
    size_t byte_offset = 10;
    
    // Client identifier length
    size_t client_identifier_length = *(message + byte_offset) << 8 | *(message + (byte_offset + 1));
    LOG(DEBUG) << "Payload --> Client Identifier Length: " << client_identifier_length;
    
    // Client identifier
    char client_identifier_bytes[client_identifier_length+1] = {0};
    memcpy(client_identifier_bytes, message + (byte_offset + 2), client_identifier_length+1);
    connect_payload->client_identifier = client_identifier_bytes;
    LOG(DEBUG) << "Payload --> Client Identifier: " << connect_payload->client_identifier;

    if(connect_flags->will_flag){
        
        // Will topic length
        byte_offset += 2 + client_identifier_length;
        size_t will_topic_length = *(message + byte_offset) << 8 | *(message + (byte_offset + 1));
        LOG(DEBUG) << "Payload --> Will Topic Length: " << will_topic_length;
        
        // Will topic
        char will_topic_bytes[will_topic_length+1] = {0};
        memcpy(will_topic_bytes, message + (byte_offset + 2), will_topic_length+1);
        connect_payload->will_topic = will_topic_bytes;
        LOG(DEBUG) << "Payload --> Will Topic: " << connect_payload->will_topic;
        
        // Will message length
        byte_offset += 2 + will_topic_length;
        size_t will_message_length = *(message + byte_offset) << 8 | *(message + (byte_offset + 1));
        LOG(DEBUG) << "Payload --> Will Message Length: " << will_message_length;
        
        // Will message
        char will_message_bytes[will_topic_length+1] = {0};
        memcpy(will_message_bytes, message + (byte_offset + 2), will_message_length+1);
        connect_payload->will_message = will_message_bytes;
        LOG(DEBUG) << "Payload --> Will Message: " << connect_payload->will_message;
        
        byte_offset += 2 + will_message_length;
    }
    
    if(connect_flags->username_flag){
        // User name length
        size_t username_length = *(message + byte_offset) << 8 | *(message + (byte_offset + 1));
        LOG(DEBUG) << "Payload --> Username Length: " << username_length;    
        
        // Username
        char username_bytes[username_length+1] = {0};
        memcpy(username_bytes, message + (byte_offset + 2), username_length+1);
        connect_payload->username = username_bytes;
        LOG(DEBUG) << "Payload --> Username: " << connect_payload->username;
        
        byte_offset += 2 + username_length;
    }
    
    if(connect_flags->password_flag){
        // Password length
        size_t password_length = *(message + byte_offset) << 8 | *(message + (byte_offset + 1));
        LOG(DEBUG) << "Payload --> Password Length: " << password_length;    
        
        // Password
        char password_bytes[password_length+1] = {0};
        memcpy(password_bytes, message + (byte_offset + 2), password_length);
        connect_payload->password = password_bytes;
        LOG(DEBUG) << "Payload --> Password: " << connect_payload->password;
    }

    // Initialize connect control packet and set variable header and payload
    ConnectControlPacket* connect_control_packet = new ConnectControlPacket();
    connect_control_packet->variable_header = variable_header;
    connect_control_packet->payload = connect_payload;
    
    // Return the connect control packet
    return connect_control_packet;
}
