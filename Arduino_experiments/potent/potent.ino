/*
  ESP32 Analog Read
  Read potentiometer voltage at different inputs

  Written by Ahmad Shamshiri at 20:55
  on Sunday Sep 01, 2019 in Ajax, Ontario, Canada

  Watch Video instruction for this sketch: 
https://youtu.be/UXKu2Xtwj7c
 
*/

//#define BLYNK_PRINT Serial

//#include "WiFi.h"
//#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char ssid[] = "Universe2";
char pass[] =  "Karakum!!!";
char auth[] = "7iSceGB4_iZkWKh3zxibWyFLzHh8nzvO";

const int potPin = 33;// Analog pin

// the number of the LED pin
const int ledPin = 22;  // 22 corresponds to GPIO16

int potValue;// do not change
float voltage =0;// do not change

// setting PWM properties
const int freq = 50;
const int ledChannel = 0;
const int resolution = 8;

BlynkTimer timer;

void myTimerEvent()
{
  potValue = analogRead(potPin);
  //potValue=300;
  Blynk.virtualWrite(V0, potValue);

  Serial.print("potValue:");
  Serial.println(potValue);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  potValue = analogRead(potPin);
  
  //voltage = (3.3/4095.0) * potValue;
  //Serial.print("potValue:");
  //Serial.println(potValue);
  /*
  Serial.print(" Voltage:");
  Serial.println(voltage);
  //Serial.println("V");  
  

  // increase the LED brightness
  int dutyCycle = potValue/16;
  //for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
  ledcWrite(ledChannel, dutyCycle);
    delay(15);
  //}

 
  //delay(2000); // delay in between reads for stability
 
  */

  int dutyCycle = potValue/16;
  ledcWrite(ledChannel, dutyCycle);
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
