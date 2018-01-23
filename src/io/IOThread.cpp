/*
 * IOThread.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: dev
 */

#include "IOThread.h"
#include "Connection.hpp"
#include "ControlPacketType.h"
#include "ConnectControlPacketParser.h"
#include "ControlPacketFixedHeader.h"
#include "ControlPacketFixedHeaderParser.h"
#include "ConnackControlPacketProducer.h"
#include "UnknownControlPacketTypeException.h"
#include "MalformedRemainingLengthException.h"
#include "ZeroBytesReceivedException.h"
#include "easylogging++.hpp"

#include <sys/epoll.h>
#include <stdlib.h>
#include <exception>

#define MAXEVENTS 64

using namespace std;
using namespace Networking;
using namespace IO;
using namespace MQTT::Control;
using namespace MQTT::Control::Connect;
using namespace MQTT::Control::Connack;

void IOThread::onEpollError(Connection* connection){
    /* An error has occured on this fd, or the socket is not
	 ready for reading (why were we notified then?) */
	LOG(ERROR) << "[io-thread] Handling error on epoll, closing socket "
			<< connection->getSocket();

	/* Closing the descriptor will make epoll remove it
	 from the set of descriptors which are monitored. */
	connection->closeConnection();
}

/**
 * Handles write-ready event from the kernel.
 * Flushes output buffer to the socket if anything in
 */
void IOThread::onEpollWriteReady(Connection* connection){
    
	LOG(ERROR) << "[io-thread] Handling output buffer FLUSH to the socket "
			<< connection->getSocket();
}

/**
 * Handles read-ready event from the kernel
 */
void IOThread::onEpollReadReady(Connection* connection){
    
    LOG(INFO) << "[io-thread] Handling READ from the client "
		      << connection->getPeerIp() << " on socket "
		      << connection->getSocket();    
		      		     
    try {
        
        // Try to parse fixed header from incoming bytes
        ControlPacketFixedHeader* fixed_header_ptr = ControlPacketFixedHeaderParser::parse(connection);
        LOG(INFO) << "Fixed header -> Control Package Type = " << fixed_header_ptr->control_packet_type;
        LOG(INFO) << "Fixed header -> Control Package Flags  " << fixed_header_ptr->control_packet_flags;
        LOG(INFO) << "Fixed header -> Remaining Length = " << fixed_header_ptr->remaining_length;
                
        // Housekeeping
        delete fixed_header_ptr;
        
    } catch (const exception& e){
                
        LOG(ERROR) << e.what();
        
        // Close connection
		connection->closeConnection();
    }
  
}

void* IOThread::run() {

    // Allocate memory for the epoll event struct array
	struct epoll_event *events = (epoll_event *) calloc(MAXEVENTS,
			sizeof(struct epoll_event));

	while (!m_graceful_stop) {

		// Get epoll events
		int num_events = epoll_wait(m_epoll_fd, events, MAXEVENTS, -1);
		LOG(INFO) << "[io-thread] Handling " << num_events << " epoll events";

		for (int i = 0; i < num_events; i++) {

			// Get tcp connection from event data pointer
			Connection* connection = (Connection*) events[i].data.ptr;

			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
					|| (events[i].events & EPOLLRDHUP)) {
					    
                // Handle epoll error
				onEpollError(connection);
                // Continue to the next event
				//continue;

            } else if(events[i].events & EPOLLOUT){
                
                /* Flush out buffer to the socket */
               onEpollWriteReady(connection);
               // Continue to the next event
               continue;
            
			} else if(events[i].events & EPOLLIN){

				/* We have data on the socket waiting to be read. Read and
				 display it. We must read whatever data is available
				 completely, as we are running in edge-triggered mode
				 and won't get a notification again for the same
				 data. */
				
				onEpollReadReady(connection);
/*
								    
				    if(message_length > 0){
				        
    					LOG(INFO) << "IO Thread " << m_tid << " obtaining total of "
    							<< message_length << " message bytes from "
    							<< connection->getPeerIp() << " on socket "
    							<< connection->getSocket();
    
    					char msgbuff[message_length];
    
    					int bytes_received = 0;
    					bool handleNextEvent = false;
    
    					while (bytes_received < message_length) {
    
    						// Receive bytes
    						int rcvd = connection->receive(msgbuff, message_length);
    
    						if (rcvd == 0) {
    							LOG(INFO) << "IO thread " << m_tid
    									<< " handling closed connection from client "
    									<< connection->getPeerIp() << " on socket "
    									<< connection->getSocket();
    
    							// Close connection
    							connection->closeConnection();
    
    							handleNextEvent = true;
    							break;
    						}
    
    						bytes_received += rcvd;
    
    					};
    
    					if (handleNextEvent) {
    						// Continue to next event
    						continue;
    					}
    					
    					if (msg_type == MqttControlPacketType::CONNECT) {
    						// Handling CONNECT message
    
    						LOG(INFO) << "IO Thread (" << m_tid
    								<< ") handling CONNECT message from client "
    								<< connection->getPeerIp() << " on socket "
    								<< connection->getSocket();
    
    						// Handle message and return thr CONNACK
    						ConnectControlPacket* connectControlPacket = ConnectControlPacketParser::parse(msgbuff);
    						
    						// Connack control package fixed header
    						ControlPacketFixedHeader* connack_fixed_header = new ControlPacketFixedHeader();
    						connack_fixed_header->control_packet_type = MqttControlPacketType::CONNACK;
    						connact_fixed_header->control_packet_flags = 0;
    						connack_fixed_header->remaining_length = 2;
    						
    						// Connect acknowledge flags
    						ConnectAcknowledgeFlags* connect_ack_flags = new ConnectAcknowledgeFlags();
    						connect_ack_flags->session_present = 0;
    						
    						// Connack control package variable header
    						ConnackVariableHeader* connack_variable_header = new ConnackVariableHeader();
    						connack_variable_header->acknowledge_flags = connect_ack_flags;
    						connack_variable_header->connect_return_code = ConnectReturnCode::CONNECTION_ACCEPTED;
    						
    						// Initialize connack control package
    						ConnackControlPacket* connack_control_package = new ConnackControlPacket();
    						connack_control_package->fixed_header = connack_fixed_header;
    						connack_control_package->variable_header = connack_variable_header;
    						
    						char* connack_bytes = ConnackControlPacketProducer::serialize(connack_control_package);
    						// Send CONNACK to the client
    						connection->send(connack_bytes, sizeof connack_bytes);
    						
    					}
				        				        
				    } else {
				        
    					if (msg_type == MqttControlPacketType::DISCONNECT){
    					    
    					    LOG(INFO) << "IO Thread (" << m_tid
    								<< ") handling DISCONNECT message from client "
    								<< connection->getPeerIp() << " on socket "
    								<< connection->getSocket();
    					    
    					}
				    }


				}
*/
                if(connection->isClosed() == false){
    				// Reset what we are watching for
    				events[i].events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    				// tell epoll to re-enable this fd.
    				int result = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD,
    						connection->getSocket(), &events[i]);
    
    				if (result == -1) {
    					LOG(ERROR) << "[io-thread] epoll_ctl() failed";
    					// Close connection
    					connection->closeConnection();
    					continue;
    				}
                }

			}

		}
	}

	return NULL;

}

