#include <iostream>
#include "cheerinos.h"

using namespace std;

// instantiate receive handler
void buf_received(char* buffer, uint8_t size);

// instatiate cheerinos object with its node address
cheerinos ch(1);

int main() {

  // set up data to send
  uint8_t to_addr = 2;
  char data_to_send[] = "hello world";

  // register receive handler
  ch.register_handler(&buf_received);
  std::cout<<"main: registered receive handler"<<std::endl;

  // send data!
  // NB. + 1 after strlen is because ascii strings are nul terminated
  ch.send(to_addr, data_to_send, strlen(data_to_send) + 1);
  std::cout<<"main: sent packed with payload - "<<data_to_send<<std::endl;
  std::cout<<std::endl<<"main: -----------------------" <<std::endl;

  // set up simulation for serial receive simulator
  std::cout<<"main: starting receive simulator" <<std::endl;
  std::cout<<"main: -----------------------" <<std::endl;

  // loop 50 times
  for (int i=0; i<50; i++) {
    ch.update();
  };
  return 0;
}

// implement receive handler
void buf_received(char* buffer, uint8_t size) {
  std::cout<<"RECEIVE HANDLER: Packet Received! "<<std::endl;
  std::cout<<"RECEIVE HANDLER: printing below... "<<std::endl<<std::endl;
  ch.sm_debug_packet(buffer, size);
}
