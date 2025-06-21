#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"


// create a pixel strand with 1 pixel on PIN_NEOPIXEL
Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);

Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(A3, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

void setup() {
    Serial.begin(115200);
    pixels.begin();  // initialize the pixel

    if (! qt_1.begin()) {
      Serial.println("Failed to begin qt on pin A0");
    }
}

void loop() {
    int result = 0;
  
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    double hue = 1.0, saturation = 1.0, value = 0.1;

    for(hue = 0; hue <= 1; hue += 0.01) {



      
      HsvToRgb(hue, saturation, value, red, green, blue);
      
      // set the first pixel #0 to red
      pixels.setPixelColor(0, pixels.Color(red, green, blue));
      // and write the data
      pixels.show();

      do {
        result = qt_1.measure(); 
        Serial.println(result);
      } while (result < 250);
      

    
      delay(10);


    }
}

void HsvToRgb(double hue, double saturation, double value, uint8_t& red, uint8_t& green, uint8_t& blue)
{
  double r, g, b;

  auto i = static_cast<int>(hue * 6);
  auto f = hue * 6 - i;
  auto p = value * (1 - saturation);
  auto q = value * (1 - f * saturation);
  auto t = value * (1 - (1 - f) * saturation);

  switch (i % 6)
  {
  case 0: r = value , g = t , b = p;
    break;
  case 1: r = q , g = value , b = p;
    break;
  case 2: r = p , g = value , b = t;
    break;
  case 3: r = p , g = q , b = value;
    break;
  case 4: r = t , g = p , b = value;
    break;
  case 5: r = value , g = p , b = q;
    break;
  }

  red = static_cast<uint8_t>(r * 255);
  green = static_cast<uint8_t>(g * 255);
  blue = static_cast<uint8_t>(b * 255);
}
