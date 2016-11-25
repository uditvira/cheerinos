/*
Author: Udit Vira
Date: November 24th, 2016

*/

#ifndef RING_BYTE_BUF_H
#define RING_BYTE_BUF_H

#ifndef MAX_BUF_LEN
#define MAX_BUF_LEN 64
#endif

class ring_byte_buf {

 public:
  ring_byte_buf();
  void enqueue(char incoming);
  char dequeue();
  unsigned int get_fill_level();

  void debug_dump();

 private:
  char ring_buf[MAX_BUF_LEN];
  unsigned int enqueue_index;
  unsigned int dequeue_index;
  void increment_index(unsigned int &index);

};

#endif
