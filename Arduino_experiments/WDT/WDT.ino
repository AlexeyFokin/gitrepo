#include "Wire.h"


Wire.beginTransmission(0x50); 
Wire.write(address);
Wire.write(data);
Wire.endTransmission();    // stop transmitting




// Set up the WDT to perform a system reset if the loop() blocks for more than 1 second
void setup() 
{
 // Set up the generic clock (GCLK2) used to clock the watchdog timer at 1.024kHz
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(4) |            // Divide the 32.768kHz clock source by divisor 32, where 2^(4 + 1): 32.768kHz/32=1.024kHz
                    GCLK_GENDIV_ID(2);              // Select Generic Clock (GCLK) 2
  while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_DIVSEL |          // Set to divide by 2^(GCLK_GENDIV_DIV(4) + 1)
                     GCLK_GENCTRL_IDC |             // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |           // Enable GCLK2
                     GCLK_GENCTRL_SRC_OSCULP32K |   // Set the clock source to the ultra low power oscillator (OSCULP32K)
                     GCLK_GENCTRL_ID(2);            // Select GCLK2         
  while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

  // Feed GCLK2 to WDT (Watchdog Timer)
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |           // Enable GCLK2 to the WDT
                     GCLK_CLKCTRL_GEN_GCLK2 |       // Select GCLK2
                     GCLK_CLKCTRL_ID_WDT;           // Feed the GCLK2 to the WDT
  while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

  REG_WDT_CONFIG = WDT_CONFIG_PER_2K;              // Set the WDT reset timeout to 1 second
  while(WDT->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
  REG_WDT_CTRL = WDT_CTRL_ENABLE;                   // Enable the WDT in normal mode
  while(WDT->STATUS.bit.SYNCBUSY);                  // Wait for synchronization

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(115200);
  Serial.println("setup");
}

int buttonState = 0;

void loop()
{
  if (!WDT->STATUS.bit.SYNCBUSY)                // Check if the WDT registers are synchronized
  {
    REG_WDT_CLEAR = WDT_CLEAR_CLEAR_KEY;        // Clear the watchdog timer
  }
  // Application code goes here...

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (HIGH is the voltage level)
  delay(250);                       // wait for a long time while the LED remains off
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED on by making the voltage LOW
  delay(250); 
  // wait for a short time to briefly light the LED


  buttonState = digitalRead(2);
  Serial.println(buttonState);
  while(buttonState == 0)
  {
    buttonState = digitalRead(2);
    Serial.println(buttonState);
    delay(100);
  }
}
