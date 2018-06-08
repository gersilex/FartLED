#include <stdio.h>
#include <Arduino.h>
#include <FastLED.h>
#include <SPI.h>

// Physical constants
#define DATA_PIN 6
#define CELL_SIZE 3
#define NUM_LEDS 54

// Application onstants
#define NUM_CELLS NUM_LEDS / CELL_SIZE
#define FRAMES 60
#define FRAMETIME 1000 / FRAMES

CRGB leds[NUM_LEDS];
CRGB ledColor = CRGB::White;
int currentCell = 1;
int previousCell = 0;

int directionCurrent = 1;
int directionPrevious = 1;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_packet_t;

void setCell(int cellId, CRGB value) {
    for (int cellCounter = 0; cellCounter < CELL_SIZE; cellCounter++) {
        int currentLED = (cellId * CELL_SIZE) + cellCounter;
        if (currentLED < NUM_LEDS) {
            leds[(cellId * CELL_SIZE) + cellCounter] = value;
        }
    }
}

void blink(CRGB color) {
    for (int n = 0; n < NUM_LEDS; n++) {
        leds[n] = color;
    }
    FastLED.show();

    delay(200);
    for (int n = 0; n < NUM_LEDS; n++) {
        leds[n] = CRGB::Black;
    }
    FastLED.show();

    delay(200);
}

void intro() {
    for (int n = 0;n < NUM_LEDS; n++) {
        leds[n] = CRGB::White;

        FastLED.show();
        delay(FRAMETIME / 2);
    }
    delay(500);
    blink(CRGB::Black);
    blink(CRGB::Red);
    blink(CRGB::Green);
    blink(CRGB::Blue);
}

void setup()
{
    Serial.begin(9600);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    //intro();
}

void serialEvent() {
    uint8_t led_index = 0;
    String input;
    while (Serial.available() && led_index < NUM_LEDS) {
        input = Serial.readStringUntil(',');
        uint32_t color = strtoul(input.c_str(), nullptr, 16);
        leds[led_index++].setColorCode(color);

        // leds[led_index++].setRGB(
        //     Serial.read(),
        //     Serial.read(),
        //     Serial.read()
        // );

        FastLED.show();
        delay(2);
    }

    while (Serial.available()) {
        Serial.read();
    }
}

void loop()
{
    // setCell(currentCell+=directionCurrent, CRGB::Red);
    // setCell(previousCell+= directionPrevious, CRGB::Black);

    // if (currentCell == NUM_CELLS) {
    //     directionCurrent = -1;
    // } else if(currentCell == 0) {
    //     directionCurrent = 1;
    // }

    // if (previousCell == NUM_CELLS) {
    //     directionPrevious = -1;
    // } else if(previousCell == 0) {
    //     directionPrevious = 1;
    // }

    FastLED.show();
    delay(FRAMETIME);
}

