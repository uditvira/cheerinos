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
+ Move simulated serial class out on main library
+ Include a byte ring buffer for serial simulator
+ Test with 2 instances of cheerinos

## To run tests:
1. First build with g++
   + `g++ -o test cheerinos.cpp test.cpp`
2. Run test
   + `./test`

