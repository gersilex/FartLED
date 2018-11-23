#pragma once
#include <Arduino.h>
#include <Ethernet.h>

struct networkConfig {
  uint8_t listenPort = 23;
  IPAddress listenAddress = IPAddress(192, 168, 59, 222);
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  EthernetServer server;
};

class Networking
{
private:
  networkConfig config;
  boolean alreadyConnected;
public:
  Networking(networkConfig config);
  // ~Networking();

  void begin();
  const char* read(EthernetServer server);
};
