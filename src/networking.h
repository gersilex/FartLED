#pragma once
#include <Arduino.h>

#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = {
  0xB0, 0x0B, 0x13, 0x5B, 0x16, 0x05
};

unsigned int localPort = 8888;      // local port to listen on

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

EthernetUDP udp;

void initializeUpdConnection(uint8_t first, uint8_t second, uint8_t third, uint8_t last);
