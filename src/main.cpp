#include <SPI.h>
#include <Ethernet.h>
#include <FastLED.h>

/////////////// Configuration ///////////////
// Networking
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress fallbackIP(192, 168, 59, 222);
#define DHCP_TIMEOUT_MS 10000L
#define LED_STREAM_PORT 41
#define CONTROL_PORT 23
#define PROTOCOL_VERSION 0

// LED Setup
#define LED_COUNT 150
#define LED_DATA_PIN 6
#define CELL_SCALE 2
#define ROWS 8
#define COLS 8
// #define LED_TEST_AT_BOOT
/////////////////////////////////////////////

EthernetServer ledStreamServer(41);
bool usingDhcp = false;

CRGB leds[LED_COUNT];

// void statusLed(uint8_t cell, colorstructTODO color){
//   TODO: For the setup phase, provide a function to set a cell to a specific color to be able to visualize the setup step and occured problems.
//   TODO: The color struct is yet to be created
// }

void setStatusRow(uint8_t row, CRGB color)
{
  uint8_t position = row * COLS * CELL_SCALE;
  for (uint8_t i = 0; i < COLS * CELL_SCALE; i++)
  {
    leds[position + i] = color;
  }
  FastLED.show();
}

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
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, LED_COUNT);

#ifdef LED_TEST_AT_BOOT
  Serial.print("Testing LEDs: ");

  Serial.print("[all] ");
  for (uint16_t i = 4; i <= 255; i *= 2)
  {
    FastLED.showColor(CRGB::White, i);
    delay(500);
  }
  delay(1000);

  Serial.print("[single] ");
  for (int i = LED_COUNT; i >= 0; --i)
  {
    leds[i] = CRGB::White;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[i] = CRGB::Black;
    delay(100);
  }

  Serial.print("[R] ");
  FastLED.showColor(CRGB::Red);
  delay(1000);
  Serial.print("[G] ");
  FastLED.showColor(CRGB::Green);
  delay(1000);
  Serial.print("[B] ");
  FastLED.showColor(CRGB::Blue);
  delay(1000);

  FastLED.clear();
  FastLED.show();
  Serial.println("done.");
#endif

  setStatusRow(0, CRGB::Green);
}

void networkSetup()
{
  setStatusRow(1, CRGB::Blue);
  Serial.print("Broadcasting DHCP Request... ");
  if (Ethernet.begin(mac, DHCP_TIMEOUT_MS) == 1)
  {
    setStatusRow(1, CRGB::Yellow);
    Serial.print("succeeded: ");
    Serial.println(Ethernet.localIP());
    usingDhcp = true;
  }
  else
  {
    setStatusRow(1, CRGB::Orange);
    Serial.print("failed. Falling back to hard-coded static IP: ");
    Serial.println(fallbackIP);
  }

  ledStreamServer.begin();
  if (Ethernet.hardwareStatus() == EthernetHardwareStatus::EthernetNoHardware)
  {
    Serial.println("ERROR: Starting the TCP/IP server failed: No hardware detected.");
    setStatusRow(1, CRGB::Red);
  }
  else
  {
    setStatusRow(1, CRGB::Green);
    Serial.print("Ready to receive LED stream data on tcp/");
    Serial.print(LED_STREAM_PORT);
    Serial.println(".");
  }
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
    Serial.println(client.peek(), 16);

    byte incoming = client.read();
    byte incomingProtocolVersion = (incoming & 0xF0) >> 4;
    byte incomingCellScale = incoming & 0x0F;

    Serial.print("Client presents header with protocol version: ");
    Serial.println(incomingProtocolVersion);
    if (incomingProtocolVersion != PROTOCOL_VERSION)
    {
      Serial.print("ERROR: Protocol mismatch. Client protocol version ");
      Serial.print(incomingProtocolVersion);
      Serial.print(" is incompatible with this server protocol version ");
      Serial.print(PROTOCOL_VERSION);
      Serial.println(". Dropping connection.");
      client.stop();
    }

    Serial.print("Client requests cell scale of: ");
    Serial.println(incomingCellScale);

    uint8_t incomingMessageLength = client.read();
    Serial.print("Client presents message length of: ");
    Serial.println(incomingMessageLength);

    Serial.print("Ready to receive ");
    Serial.print(incomingMessageLength * 3);
    Serial.println(" bytes.");

    for (uint16_t i = 0; i < incomingMessageLength * 3; i++)
    {
      Serial.print(client.read(), 16);
      Serial.print(',');
    }
    Serial.println();
    Serial.println("--- End of message ---");
  }
}

void entropySetup()
{
  pinMode(A5, INPUT);
  randomSeed(analogRead(A5));
  Serial.print("Random seed: ");
  Serial.println(random(100));
}

void setup()
{
  serialSetup();
  ledSetup();
  networkSetup();
  delay(1000);
}

void loop()
{
  if (usingDhcp)
  {
    Ethernet.maintain();
  }

  handleLedStream();
}
