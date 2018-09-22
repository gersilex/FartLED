// Funktioniert nur im 59er Netz, an Wilkos Kabel
// Hier kein subnetz, dns oder gw rein, weil wir nicht antworten wollen

#include <Arduino.h>
#include <SPI.h>

#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = {
  0xB0, 0x0B, 0x13, 0x5B, 0x16, 0x05
};

unsigned int localPort = 8888;      // local port to listen on

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

EthernetUDP udp;

void initializeUpdConnection(uint8_t first, uint8_t second, uint8_t third, uint8_t last) {

  Ethernet.begin(
    mac,
// An EthernetUDP instance to let us send and receive packets over UDP
    IPAddress(first, second, third, last)
  );
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("GW: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.print("Subnet: ");
  Serial.println(Ethernet.subnetMask());
  // give the Ethernet shield a second to initialize:
  delay(1000);
  udp.begin(localPort);
}

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
