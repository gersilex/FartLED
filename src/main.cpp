#include <SPI.h>
#include <Ethernet.h>

/////////////// Configuration ///////////////
// Networking
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress fallbackIP(192, 168, 59, 222);
#define DHCP_TIMEOUT_MS 1000L
#define LED_STREAM_PORT 41
#define CONTROL_PORT 23
/////////////////////////////////////////////

EthernetServer ledStreamServer(41);
bool usingDhcp = false;

// void statusLed(uint8_t cell, colorstructTODO color){
//   TODO: For the setup phase, provide a function to set a cell to a specific color to be able to visualize the setup step and occured problems.
//   TODO: The color struct is yet to be created
// }

void serialSetup()
{
  Serial.begin(9600);
  Serial.println(" :: FartLED :: ");
  Serial.println("Serial console started.");
}

void ledSetup()
{
  // stub
  // TODO: Configure FastLED, Power ON all colors of all LEDs, then cycle through only Red, only Green and only Blue, wait 1 seconds between those 4 steps
}

void networkSetup()
{
  Serial.print("Broadcasting DHCP Request... ");
  if (Ethernet.begin(mac, DHCP_TIMEOUT_MS) == 1)
  {
    Serial.print("succeeded: ");
    Serial.println(Ethernet.localIP());
    usingDhcp = true;
  }
  else
  {
    Serial.print("failed. Falling back to hard-coded static IP: ");
    Serial.println(fallbackIP);
  }

  ledStreamServer.begin();
  Serial.print("Ready to receive LED stream data on tcp/");
  Serial.print(LED_STREAM_PORT);
  Serial.println(".");
}

void handleLedStream()
{
  EthernetClient client = ledStreamServer.available(); // A client connected

  if (client) // Is the client sending data?
  {
    Serial.print("Client ");
    Serial.print(client.remoteIP());
    Serial.print(":");
    Serial.print(client.remotePort());
    Serial.print(" sent: ");
    Serial.println(client.read());
  }
}

void setup()
{
  serialSetup();
  ledSetup();
  networkSetup();
}

void loop()
{
  if (usingDhcp)
  {
    Ethernet.maintain();
  }

  handleLedStream();
}
