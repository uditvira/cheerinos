/*
Author: Udit Vira
Date: November 24th, 2016

*/

#ifndef  SIM_SERIAL_H
#define SIM_SERIAL_H

#define MAX_BUF_LEN 64

#include "ring_byte_buf.h"

class sim_serial {

 public:

  // public class interface
  int available();
  char read();
  void write(char incoming);
  void write(char* buf, unsigned int len);

  // connection interface (software defined connections)
  void tx_out(char* buf, unsigned int &len);
  void rx_in(char* buf, unsigned int len);

 private:
  // RX side
  // read from the rx side
  ring_byte_buf rx_buf;

  // TX side
  // write to tx_side
  ring_byte_buf tx_buf;

};

#endif
