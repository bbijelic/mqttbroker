#include <iostream>

#include "easylogging++.hpp"
#include "ConnectControlPacketParser.h"

using namespace std;
using namespace MQTT::Control::Connect;

ConnectControlPacket* ConnectControlPacketParser::parse(char* message){
    LOG(INFO)<< "Parsing CONNECT message";
    
    // variable header instance
    ConnectVariableHeader* variableHeader = new ConnectVariableHeader();
    
    // Protocol name
    char protocol_name[4] = {0};
    memcpy(protocol_name, message + 2, 4);
    variableHeader->protocol_name = string(protocol_name); 
    LOG(DEBUG) << "Variable Header --> Protocol name: " << variableHeader->protocol_name;
    
    // Protocol level
    variableHeader->protocol_level = message[6];
    LOG(DEBUG) << "Variable Header --> Protocol level: " << variableHeader->protocol_level;
        
    // Connect flags
    ConnectFlags* connectFlags = new ConnectFlags();
    char connectFlagByte = message[7];
    
    // Reserved flag
    connectFlags->reserved = (bool)(0x01 & connectFlagByte);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Reserved: " << connectFlags->reserved;
    
    // Clean session flag
    connectFlags->clean_session = (bool)((0x02 & connectFlagByte) >> 1);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Clean Session: " << connectFlags->clean_session;
    
    // Will flag
    connectFlags->will_flag = (bool)((0x04 & connectFlagByte) >> 2);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Will Flag: " << connectFlags->will_flag;
    
    // Will QoS
    connectFlags->will_qos = (unsigned short)((0x18 & connectFlagByte) >> 3);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Will QoS: " << connectFlags->will_qos;
    
    // Will Retain
    connectFlags->will_retain = (bool)((0x20 & connectFlagByte) >> 5);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Will Retain: " << connectFlags->will_retain;
    
    // Password flag
    connectFlags->password_flag = (bool)((0x40 & connectFlagByte) >> 6);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Password flag: " << connectFlags->password_flag;
    
    // Username flag
    connectFlags->username_flag = (bool)((0x80 & connectFlagByte) >> 7);
    LOG(DEBUG)<< "Variable Header --> Connect Flags --> Username flag: " << connectFlags->username_flag;
    
    ConnectControlPacket* connectControlPacket = new ConnectControlPacket();
    return connectControlPacket;
}
