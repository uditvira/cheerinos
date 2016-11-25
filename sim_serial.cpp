/*
  Author: Udit Vira
  Date: November 24th, 2016

*/

#include "sim_serial.h"

int sim_serial::available() {
  return rx_buf.get_fill_level();
}

char sim_serial::read() {
  return rx_buf.dequeue();
}

void sim_serial::write(char incoming) {
  tx_buf.enqueue(incoming);
}

void sim_serial::write(char* buf, unsigned int len) {
  for (int i = 0; i < len; i++) {
    tx_buf.enqueue(buf[i]);
  }
}

void sim_serial::tx_out(char* buf, unsigned int &len) {
  len = tx_buf.get_fill_level();
  for (int i = 0; i < len; i++) {
      buf[i] = tx_buf.dequeue();
  }
}

void sim_serial::rx_in(char* buf, unsigned int len) {
  for (int i = 0; i < len; i++) {
    rx_buf.enqueue(buf[i]);
  }
}


