#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 255 

// Define the array of leds
CRGB leds[NUM_LEDS];

void lightsSetup() { 
//  Serial.begin(57600);
//  Serial.println("resetting");
    FastLED.addLeds<APA102,11, 13, BGR>(leds, NUM_LEDS);
    LEDS.setBrightness(255);
    colorall();
    fadeall();
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void colorall() {
    static uint8_t hue = 0;
    for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CRGB(255, 0, 0);
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
      leds[i] = CRGB(0, 255, 0);
      leds[i].fadeLightBy( _time % 255 );
    }
}

void loopLights() { 
  colorTime(millis());
    FastLED.show(); 
}
