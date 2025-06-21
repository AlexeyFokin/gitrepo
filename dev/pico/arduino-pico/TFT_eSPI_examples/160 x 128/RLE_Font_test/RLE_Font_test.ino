/*
 Display all the fast rendering fonts.

 This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8
 
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

//#include "pico_unique_id/include/pico/unique_id.h"
#include "pico.h"

// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 500

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0; // Used for testing draw times

void setup(void) {
  #define PWR_ON 22
  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH);

  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  tft.init();
  tft.setRotation(1);
}

void loop() {
  targetTime = millis();

  // First we test them with a background colour set
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  // tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  // tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  // tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  // int xpos = 0;
  // xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  // tft.drawChar(127, xpos, 64, 2);
  // delay(WAIT);

  // tft.fillScreen(TFT_BLACK);
  // tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  // tft.drawString("/0123456789:;", 0, 26, 4);
  // tft.drawString("<=>?@ABCDE", 0, 52, 4);
  // tft.drawString("FGHIJKLMNO", 0, 78, 4);
  // tft.drawString("PQRSTUVWX", 0, 104, 4);
  // delay(WAIT);

  // tft.fillScreen(TFT_BLACK);
  // tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  // tft.drawString("defghijklmno", 0, 26, 4);
  // tft.drawString("pqrstuvwxyz", 0, 52, 4);
  // xpos = 0;
  // xpos += tft.drawString("{|}~", 0, 78, 4);
  // tft.drawChar(127, xpos, 78, 4);
  // delay(WAIT);

  // tft.fillScreen(TFT_BLACK);
  // tft.setTextColor(TFT_BLUE, TFT_BLACK);


  int b1 = digitalRead(6);
  int b2 = digitalRead(7);

  tft.drawString(String(b1), 0, 0, 4);

  static char board_id_string[17];
  pico_get_unique_board_id_string(&board_id_string[0], 17);
  
  tft.drawString(&board_id_string[0], 0, 40, 4);
  //tft.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  // tft.fillScreen(TFT_BLACK);
  // tft.setTextColor(TFT_RED, TFT_BLACK);

  // tft.drawString("0123", 0, 0, 7);
  // tft.drawString("4567", 0, 60, 7);
  // delay(WAIT);

  // tft.fillScreen(TFT_BLACK);
  // tft.drawString("890:.", 0, 0, 7);
  // tft.drawString("", 0, 60, 7);
  // delay(WAIT);

  // tft.fillScreen(TFT_BLACK);
  // tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  // tft.drawString("01", 0, 0, 8);
  // delay(WAIT);
  
  // tft.drawString("23", 0, 0, 8);
  // delay(WAIT);

  // tft.drawString("45", 0, 0, 8);
  // delay(WAIT);
  
  // tft.drawString("67", 0, 0, 8);
  // delay(WAIT);

  // tft.drawString("89", 0, 0, 8);
  // delay(WAIT);
 
  // tft.drawString("0:.", 0, 0, 8);
  // delay(WAIT);

  // tft.setTextColor(TFT_MAGENTA);
  // tft.drawNumber(millis() - targetTime, 0, 100, 4);
  // delay(4000);

  // // Now test them with transparent background
  // targetTime = millis();

  // tft.setTextSize(1);
  // tft.fillScreen(TFT_BROWN);
  // tft.setTextColor(TFT_GREEN);

  // tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  // tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  // tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  // tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  // xpos = 0;
  // xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  // tft.drawChar(127, xpos, 64, 2);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.setTextColor(TFT_GREEN);

  // tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  // tft.drawString("/0123456789:;", 0, 26, 4);
  // tft.drawString("<=>?@ABCDE", 0, 52, 4);
  // tft.drawString("FGHIJKLMNO", 0, 78, 4);
  // tft.drawString("PQRSTUVWX", 0, 104, 4);

  // delay(WAIT);
  // tft.fillScreen(TFT_BROWN);
  // tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  // tft.drawString("defghijklmno", 0, 26, 4);
  // tft.drawString("pqrstuvwxyz", 0, 52, 4);
  // xpos = 0;
  // xpos += tft.drawString("{|}~", 0, 78, 4);
  // tft.drawChar(127, xpos, 78, 4);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.setTextColor(TFT_BLUE);

  // tft.drawString("012345", 0, 0, 6);
  // tft.drawString("6789", 0, 40, 6);
  // tft.drawString("apm-:.", 0, 80, 6);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.setTextColor(TFT_RED);

  // tft.drawString("0123", 0, 0, 7);
  // tft.drawString("4567", 0, 60, 7);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.drawString("890:.", 0, 0, 7);
  // tft.drawString("", 0, 60, 7);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.setTextColor(TFT_YELLOW);

  // tft.drawString("0123", 0, 0, 8);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.drawString("4567", 0, 0, 8);
  // delay(WAIT);

  // tft.fillScreen(TFT_BROWN);
  // tft.drawString("890:.", 0, 0, 8);
  // delay(WAIT);

  // tft.setTextColor(TFT_MAGENTA);

  // tft.drawNumber(millis() - targetTime, 0, 100, 4);
  // delay(4000);;
}

