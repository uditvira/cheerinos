# Cheerinos

Note: this is work in progress!

Cheerinos is an Arduino serial communication library to make serial communication between python and arduino easy.

## Packet structure
+ SOH     - ASCII Start of header
+ HEADER  - Includes `from_addr`, `to_addr`, and `num_bytes` of payload
+ STX     - ASCII Start of text
+ PAYLOAD - Payload data
+ ETX     - ASCII End of text
+ CRC     - Cyclic Redundancy Check (8 bits)
+ EOT     - ASCII End of Transmission

## TODOs
+ Python Boost bindings
+ Wait for ACK/NACK
+ Implement CRC
+ Test with 2 instances of cheerinos

## To run tests:
1. First build with g++
   + `g++ -o test ring_byte_buf.cpp sim_serial.cpp cheerinos.cpp test.cpp`
2. Run test
   + `./test`

