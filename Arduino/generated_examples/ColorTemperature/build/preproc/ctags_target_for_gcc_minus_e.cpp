# 1 "/home/alexey/Arduino/generated_examples/ColorTemperature/ColorTemperature.ino"
/// @file    ColorTemperature.ino
/// @brief   Demonstrates how to use @ref ColorTemperature based color correction
/// @example ColorTemperature.ino

# 6 "/home/alexey/Arduino/generated_examples/ColorTemperature/ColorTemperature.ino" 2



// Information about the LED strip itself



CRGB leds[16];




// FastLED provides two color-management controls:
//   (1) color correction settings for each LED strip, and
//   (2) master control of the overall output 'color temperature' 
//
// THIS EXAMPLE demonstrates the second, "color temperature" control.
// It shows a simple rainbow animation first with one temperature profile,
// and a few seconds later, with a different temperature profile.
//
// The first pixel of the strip will show the color temperature.
//
// HELPFUL HINTS for "seeing" the effect in this demo:
// * Don't look directly at the LED pixels.  Shine the LEDs aganst
//   a white wall, table, or piece of paper, and look at the reflected light.
//
// * If you watch it for a bit, and then walk away, and then come back 
//   to it, you'll probably be able to "see" whether it's currently using
//   the 'redder' or the 'bluer' temperature profile, even not counting
//   the lowest 'indicator' pixel.
//
//
// FastLED provides these pre-conigured incandescent color profiles:
//     Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc,
//     HighNoonSun, DirectSunlight, OvercastSky, ClearBlueSky,
// FastLED provides these pre-configured gaseous-light color profiles:
//     WarmFluorescent, StandardFluorescent, CoolWhiteFluorescent,
//     FullSpectrumFluorescent, GrowLightFluorescent, BlackLightFluorescent,
//     MercuryVapor, SodiumVapor, MetalHalide, HighPressureSodium,
// FastLED also provides an "Uncorrected temperature" profile
//    UncorrectedTemperature;




// How many seconds to show each temperature before switching

// How many seconds to show black between switches


void loop()
{
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, 16 - 5, --starthue, 20);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (20 * 2);
  if( secs < 20) {
    FastLED.setTemperature( Tungsten100W ); // first temperature
    leds[0] = Tungsten100W; // show indicator pixel
  } else {
    FastLED.setTemperature( OvercastSky ); // second temperature
    leds[0] = OvercastSky; // show indicator pixel
  }

  // Black out the LEDs for a few secnds between color changes
  // to let the eyes and brains adjust
  if( (secs % 20) < 3) {
    memset( leds, 0, 16 * sizeof(CRGB));
  }

  FastLED.show();
  FastLED.delay(8);
}

void setup() {
  delay( 3000 ); // power-up safety delay
  // It's important to set the color correction for your LED strip here,
  // so that colors can be more accurately rendered through the 'temperature' profiles
  FastLED.addLeds<WS2812B, 18, GRB>(leds, 16).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( 30 );
}
