/*
  Author: Udit Vira
  Date: November 24th, 2016

*/

#include "ring_byte_buf.h"
#include <iostream>

ring_byte_buf::ring_byte_buf() {
  enqueue_index = 0;
  dequeue_index = 0;
}

void ring_byte_buf::enqueue(char incoming) {
  if (get_fill_level() < MAX_BUF_LEN - 1) {
    ring_buf[enqueue_index] = incoming;
    increment_index(enqueue_index);
  }
}

char ring_byte_buf::dequeue() {
  char out_char;
  if (get_fill_level() > 0) {
    out_char = ring_buf[dequeue_index];
    increment_index(dequeue_index);
    return out_char;
  }
  return (char) 0;
}

unsigned int ring_byte_buf::get_fill_level() {
  return (MAX_BUF_LEN + enqueue_index - dequeue_index) % (MAX_BUF_LEN);
}

void ring_byte_buf::increment_index(unsigned int &index) {
  index = (index + 1) % MAX_BUF_LEN;
}

void ring_byte_buf::debug_dump() {
  std::cout<<"Dumping ring buffer"<<std::endl;
  for (int i = 0; i < MAX_BUF_LEN; i++)
    std::cout<<ring_buf[i];
  std::cout<<std::endl;
  std::cout<<"enqueue index is: "<<enqueue_index<<std::endl;
  std::cout<<"dequeue index is: "<<dequeue_index<<std::endl;
  std::cout<<"fill level    is: "<<get_fill_level()<<std::endl;
}
