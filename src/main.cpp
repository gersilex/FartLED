// Funktioniert nur im 59er Netz, an Wilkos Kabel
// Hier kein subnetz, dns oder gw rein, weil wir nicht antworten wollen

#include <Arduino.h>
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP udp;

void initializeUpdConnection(uint8_t first, uint8_t second, uint8_t third, uint8_t last) {

  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  Serial.begin(9600);

  // start the Ethernet and UDP:
  Serial.println("Initialize Ethernet:");

  Ethernet.begin(
    mac,
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

    // send a reply to the IP address and port that sent us the packet we received
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.endPacket();
  }
  delay(10);
}
