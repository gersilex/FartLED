// Funktioniert nur im 59er Netz, an Wilkos Kabel
// Hier kein subnetz, dns oder gw rein, weil wir nicht antworten wollen

#include <Arduino.h>
#include <SPI.h>

#include "networking.h"

void setup() {
  Serial.begin(9600);

  Serial.println("Initialize Ethernet");
  initializeUpdConnection(192, 168, 59, 72);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = udp.remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(udp.remotePort());

    // read the packet into packetBufffer
    udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
  delay(10);
}
