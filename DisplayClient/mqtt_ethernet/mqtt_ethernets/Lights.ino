#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 255 

// Define the array of leds
CRGB leds[NUM_LEDS];

void lightsSetup() { 
//  Serial.begin(57600);
//  Serial.println("resetting");
    FastLED.addLeds<APA102,11, 13, RGB>(leds, NUM_LEDS);
    LEDS.setBrightness(255);
    colorall();
    fadeall();
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void colorall() {
    static uint8_t hue = 0;
    for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
  }
  Serial.print("x");
}

void colorTime(unsigned long _time)
{
    static uint8_t hue = 0;
    for(int i = 0; i < NUM_LEDS; i++) {
     // Set the i'th led to red 
      leds[i] = CHSV(_time % 105 + 145, 255, 255);
      // Show the leds
      FastLED.show();   
    }
}

void loopLights() { 
  colorTime(millis());
    Serial.print("x");
    FastLED.show(); 
//    fadeall();
}
