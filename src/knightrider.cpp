// #include <stdio.h>
// #include <Arduino.h>
// #include <FastLED.h>
// #include <SPI.h>

// // Physical constants
// #define DATA_PIN 6
// #define CELL_SIZE 1
// #define NUM_LEDS 54

// // Application onstants
// #define NUM_CELLS NUM_LEDS / CELL_SIZE

// CRGB leds[NUM_LEDS];
// CRGB ledColor = CRGB::White;
// int currentCell = 1;
// int previousCell = 0;

// int directionCurrent = 1;
// int directionPrevious = 1;

// void setCell(int cellId, CRGB value) {
//     for (int cellCounter = 0; cellCounter < CELL_SIZE; cellCounter++) {
//         int currentLED = (cellId * CELL_SIZE) + cellCounter;
//         if (currentLED < NUM_LEDS) {
//             leds[(cellId * CELL_SIZE) + cellCounter] = value;
//         }
//     }
// }

// void serialEvent() {
//     String input = Serial.readString();
//     unsigned long leroyDuAlteDiva = strtoul(input.c_str(), nullptr, 16);
//     ledColor.setColorCode(leroyDuAlteDiva);

//     Serial.println(leroyDuAlteDiva, 16);
// }

// void setup()
// {
//     Serial.begin(9600);
//     Serial.setTimeout(2000);
//     FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
// }

// void loop()
// {
//     setCell(currentCell+=directionCurrent, CRGB::Red);
//     setCell(previousCell+= directionPrevious, CRGB::Black);

//     if (currentCell == NUM_LEDS) {
//         directionCurrent = -1;
//     } else if(currentCell == 0) {
//         directionCurrent = 1;
//     }

//     if (previousCell == NUM_LEDS) {
//         directionPrevious = -1;
//     } else if(currentCell == 0) {
//         directionPrevious = 1;
//     }

//     FastLED.show();
//     delay(16);
// }

