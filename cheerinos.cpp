/***************************************************
Author: Udit Vira
Date: November 18th, 2016

Implementation of the cheerinos class

****************************************************/
#include <iostream>
#include "cheerinos.h"
#include "sim_serial.h"

/*********************************************
 Implementation of public functions
**********************************************/

cheerinos::cheerinos(uint8_t node_addr) {
  _node_addr = node_addr;
}

CHError cheerinos::send(uint8_t to_addr, char* buf, uint8_t size) {
  // first flush serial in to in_buffer
  while (serial.available() > 0) {
    in_buffer.enqueue(serial.read());
  }
  // build packet
  build_packet(to_addr, buf, size);
  serial.write(_cur_tx_packet, _cur_tx_packet_size);
  debug("TX cheerinos: sending packet");
  // TODO check to see if we get an ack!
  return SUCCESS;
}

void cheerinos::build_packet(uint8_t to_addr, char* buf, uint8_t size) {

  unsigned int current_index = 0;

  // calculate total packet size
  _cur_tx_packet_size = size + SIZE_OF_HEADER + SIZE_OF_CRC + NUM_OF_TX_DELIMS*BYTES_PER_DELIM;

  // start of header
  _cur_tx_packet[current_index] = (char) SOH;
  current_index += BYTES_PER_DELIM;
  // fill header
  _cur_tx_packet[current_index] = (char) _node_addr;
  current_index += SIZE_OF_ADDR;

  _cur_tx_packet[current_index] = (char) to_addr;
  current_index += SIZE_OF_ADDR;

  _cur_tx_packet[current_index] = (char) size;
  current_index += SIZE_OF_NUM_BYTES;

  // start text
  _cur_tx_packet[current_index] = (char) STX;
  current_index += BYTES_PER_DELIM;

  // fill payload
  memcpy(&(_cur_tx_packet[current_index]), buf, size);
  current_index += size;

  // end text
  _cur_tx_packet[current_index] = (char) ETX;
  current_index += BYTES_PER_DELIM;

  // calculate crc
  _cur_tx_packet[current_index] = calculate_crc(buf, size);
  current_index += SIZE_OF_CRC;

  // end of transmission
  _cur_tx_packet[current_index] = (char) EOT;

  debug("TX cheerinos: finished building packet");
}

void cheerinos::debug(char* string) {
  std::cout<<string<<std::endl;
  return;
}

void cheerinos::update() {
  char incoming;
  if (serial.available() > 0) {
    in_buffer.enqueue(serial.read());
  }

  if (in_buffer.get_fill_level() > 0) {
    incoming = in_buffer.dequeue();
    debug("RX cheerinos: found incoming byte");
    std::cout<<std::hex<<(unsigned int) incoming<<std::endl;

    if ((incoming == (char) SOH) && (sm_state == UNLOCKED))  {
      sm_state = PROCESS_HEADER;
      sm_count = 0;
      debug("RX cheerinos: entering state PROCESS_HEADER");
      return;
    } else if (sm_state == PROCESS_HEADER) {
      if (sm_count == 0) {
        sm_from = (uint8_t) incoming;
        sm_count++;
        return;
      } else if (sm_count == SIZE_OF_ADDR) {
        sm_to = (uint8_t) incoming;
        sm_count++;
        return;
      } else if (sm_count == (2*SIZE_OF_ADDR)) {
        sm_payload_size = (uint8_t) incoming;
        sm_count++;
        return;
      } else if (sm_count == (2*SIZE_OF_ADDR + SIZE_OF_NUM_BYTES) && (incoming == (char) STX)) {
        sm_count = 0;
        sm_state = PROCESS_PAYLOAD;
        debug("RX cheerinos: entering state PROCESS_PAYLOAD");
        return;
      } else {
        sm_count = 0;
        sm_state = UNLOCKED;
        debug("RX cheerinos: entering state UNLOCKED");
        return;
      }
    } else if (sm_state == PROCESS_PAYLOAD) {
      if (sm_count < sm_payload_size) {
        sm_payload[sm_count] = incoming;
        sm_count++;
        return;
      } else if ((sm_count == sm_payload_size) && (incoming == (char) ETX)) {
        sm_state = CHK_CRC;
        debug("RX cheerinos: entering state CHK_CRC");
        sm_count = 0;
        return;
      } else {
        sm_state = UNLOCKED;
        debug("RX cheerinos: entering state UNLOCKED");
        sm_count = 0;
        return;
      }
    } else if (sm_state == CHK_CRC) {
      sm_crc = (uint8_t) incoming;
        if (sm_crc == calculate_crc(sm_payload, sm_payload_size)) {
          sm_state = CHK_EOT;
          debug("RX cheerinos: entering state CHK_EOT");
        } else {
          send_nack();
          sm_state = UNLOCKED;
          debug("RX cheerinos: entering state UNLOCKED");
        }
      return;
    } else if (sm_state == CHK_EOT) {
      if (incoming == (char) EOT) {
        send_ack();
        _on_receive(sm_payload, sm_count);
      } else {
        send_nack();
      }
      sm_state = UNLOCKED;
      debug("RX cheerinos: entering state UNLOCKED");
    }
  }
}

uint8_t cheerinos::calculate_crc(char* buf, unsigned int size) {
  // TODO: this needs to be updated!!!
  return (char) 0x05;
}

void cheerinos::send_ack() {
  // TODO
  debug("Sending ACK!");
}

void cheerinos::send_nack() {
  // TODO
  debug("Sending NACK!");
}

void cheerinos::register_handler(on_receive_handler_function on_receive) {
  _on_receive = on_receive;
}

void cheerinos::sm_debug_packet(char* buf, uint8_t size) {
  std::cout<<"****************"<<std::endl;
  std::cout<<"*** PACKET *****"<<std::endl;
  std::cout<<"****************"<<std::endl;
  std::cout<<"FROM : "<<(int) sm_from <<std::endl;
  std::cout<<"TO   : "<<(int) sm_to <<std::endl;
  std::cout<<"DATA : "<<buf<<std::endl;
  std::cout<<"****************"<<std::endl<<std::endl;
}
