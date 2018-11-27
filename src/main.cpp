#include <SPI.h>
#include <Ethernet.h>
#include <FastLED.h>

/////////////// Configuration ///////////////
// Networking
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress fallbackIP(192, 168, 59, 222);
#define DHCP_TIMEOUT_MS 2000L
#define LED_STREAM_PORT 41
#define CONTROL_PORT 23
#define PROTOCOL_VERSION 0

// LED Setup
#define ROWS 8
#define COLS 8
#define CELL_SCALE 2
#define CELL_COUNT (ROWS * COLS)
#define LED_COUNT (CELL_COUNT * CELL_SCALE)
#define LED_DATA_PIN 3
#define LED_TEST_AT_BOOT
/////////////////////////////////////////////

EthernetServer ledStreamServer(41);
bool usingDhcp = false;

CRGB leds[LED_COUNT];

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
  Serial.begin(115200);
  Serial.println(" :: FartLED :: ");
  Serial.println("Serial console started.");
}

void ledSetup()
{
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, LED_COUNT);

#ifdef LED_TEST_AT_BOOT
  setStatusRow(0, CRGB::DimGray);
  Serial.print("Testing LEDs: ");

  Serial.print("[single] ");
  for (int i = LED_COUNT - 1; i >= 0; i--)
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

  Serial.print("[all] ");
  unsigned long timeout = millis() + 10000;
  while(millis() < timeout){
    fill_rainbow(leds, LED_COUNT, millis() / 10);
    FastLED.show();
  }

  Serial.print("[30s burn-in] ");
  FastLED.showColor(CRGB::White);
  delay(30000);

  FastLED.clear();
  FastLED.show();
  Serial.println("done.");
#endif

  setStatusRow(0, CRGB::Green);
}

void networkSetup()
{
  setStatusRow(1, CRGB::DimGray);
  Serial.print("Broadcasting DHCP Request... ");
  if (Ethernet.begin(mac, DHCP_TIMEOUT_MS) == 1)
  {
    Serial.print("succeeded: ");
    Serial.println(Ethernet.localIP());
    usingDhcp = true;
  }
  else
  {
    setStatusRow(1, CRGB::Blue);
    Serial.print("failed. Falling back to hard-coded static IP: ");
    Serial.println(fallbackIP);
    delay(2000);
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
  setStatusRow(2, CRGB::DimGray);
}

void handleLedStream()
{
  EthernetClient client = ledStreamServer.available(); // A client connected

  if (client) // Is the client sending data?
  {
    byte incoming = client.read();
    byte incomingProtocolVersion = (incoming & 0xF0) >> 4;
    byte incomingCellScale = incoming & 0x0F;

    Serial.print("Client presents protocol version: ");
    Serial.println(incomingProtocolVersion);
    if (incomingProtocolVersion != PROTOCOL_VERSION)
    {
      Serial.print("ERROR: Protocol mismatch. Client protocol version ");
      Serial.print(incomingProtocolVersion);
      Serial.print(" is incompatible with this server protocol version ");
      Serial.print(PROTOCOL_VERSION);
      Serial.println(". Dropping connection.");
      client.stop();
      return;
    }

    Serial.print("Client requests cell scale of: ");
    Serial.println(incomingCellScale);

    uint8_t incomingMessageLength = client.read();
    Serial.print("Client presents message length of: ");
    Serial.println(incomingMessageLength);

    Serial.print("Writing ");
    Serial.print(incomingMessageLength * 3);
    Serial.println(" bytes to framebuffer.");

    client.readBytes( (char*)leds, incomingMessageLength * 3);

    // Throw away any remaining data
    while(client.available()){
      client.read();
    }

    for(int i=0; i < LED_COUNT;i++){
      Serial.print(leds[i].red, 16);
      Serial.print(" ");
      Serial.print(leds[i].green, 16);
      Serial.print(" ");
      Serial.print(leds[i].blue, 16);
      Serial.println();
    }
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
  FastLED.show();
}
