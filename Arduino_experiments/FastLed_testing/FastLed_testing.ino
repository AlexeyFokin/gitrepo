#include <FastLED.h>
#define NUM_LEDS 10
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    // put your main code here, to run repeatedly:
  for(int i=0; i<10; i++)
  {
    leds[i] = CRGB::White;
  }
  FastLED.show(); 
}

void loop() {

 
}
