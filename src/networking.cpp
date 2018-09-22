#include "networking.h"


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
