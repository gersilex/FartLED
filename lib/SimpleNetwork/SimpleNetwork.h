#include <Arduino.h>
#include <Ethernet.h>

uint8_t listenPort = 23;
IPAddress listenAddress = IPAddress(192, 168, 59, 222);
EthernetServer server(listenPort);
byte mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
