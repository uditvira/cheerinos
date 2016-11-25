/*
Author: Udit Vira
Date: November 17th, 2016

*/
#ifndef CHEERINOS_H
#define CHEERINOS_H

#include <iostream>
#include "sim_serial.h"
#include "ring_byte_buf.h"

// NB. All sizes are in bytes.

/*********** PACKET DESCRIPTION ************
   SOH     - Start of header
   HEADER  - Includes from_addr, to_addr, and num_bytes of payload
   STX     - Start of text
   PAYLOAD - Payload data
   ETX     - End of text
   CRC     - Cyclic Redundancy Check
   EOT     - End of Transmission
*******************************************/

/************  HEADER DESCRIPTION ***********
   from_addr - Sender node address
   to_addr   - Reciever node address
   num_bytes - Number of bytes in the payload
*********************************************/

// DEFINE SIZE OF HEADER VARIABLES
#define SIZE_OF_ADDR              1
#define SIZE_OF_NUM_BYTES         1
#define SIZE_OF_HEADER            2*SIZE_OF_ADDR + SIZE_OF_NUM_BYTES

// DEFINE MAX SIZE OF PAYLOAD
#define MAX_SIZE_OF_PAYLOAD       255*SIZE_OF_NUM_BYTES

// DEFINE SIZE OF CRC
#define SIZE_OF_CRC               1

// DEFINE NUM OF TX DELIMITERS (SOH + STX + ETX + EOT)
#define NUM_OF_TX_DELIMS          4
#define BYTES_PER_DELIM           1
#define MAX_PACKET_SIZE           NUM_OF_TX_DELIMS*BYTES_PER_DELIM + SIZE_OF_HEADER + MAX_SIZE_OF_PAYLOAD

// DEFINE ASCII TRANSMISSION CHARACTERS
#define SOH 0x01
#define STX 0x02
#define ETX 0x03
#define EOT 0x04

// FOR RING BUFS
#define MAX_BUF_LEN  2*MAX_PACKET_SIZE

enum CHError {
  SUCCESS,
  FAIL
};

enum RCV_STATES {
  UNLOCKED,
  PROCESS_HEADER,
  PROCESS_PAYLOAD,
  CHK_CRC,
  CHK_EOT
};

class cheerinos {

 public:

  // typedef for handler functions
  typedef void (*on_receive_handler_function)(char* buffer, uint8_t size);

  // constructor, needs to set current node_address
  cheerinos(uint8_t node_addr);

  // send out buffers
  CHError send(uint8_t to_addr, char* buf, uint8_t size);
  
  // update
  void update();

  // register handler
  void register_handler(on_receive_handler_function on_receive);

  // debug to std out
  void debug(char* string);

  // initialize simulator
  
  void sm_debug_packet(char* buf, uint8_t size);

  // TODO:: possibly put in ifdef simulation mode
  /***********  SERIAL SIMULATOR stuff *************/
  sim_serial serial;
  /*************************************************/

 private:

  uint8_t _node_addr;

  char _cur_tx_packet[MAX_PACKET_SIZE];
  unsigned int _cur_tx_packet_size;
  
  // TODO: possibly move to sub-class??
 
  /***********  RX STATE MACHINE stuff *************/
  RCV_STATES sm_state = UNLOCKED;
  char sm_payload[MAX_PACKET_SIZE];
  uint8_t sm_from;
  uint8_t sm_to;
  uint8_t sm_crc;
  uint8_t sm_payload_size = 0;
  uint8_t sm_count = 0;
  /*************************************************/

  ring_byte_buf in_buffer;

  // handler function
  on_receive_handler_function _on_receive = NULL;

  // calculate crc-8
  uint8_t calculate_crc(char* buf, unsigned int size);
  
  // build packet
  void build_packet(uint8_t to_addr, char* buf, uint8_t size);

  void send_nack();

  void send_ack();
  
};

#endif
