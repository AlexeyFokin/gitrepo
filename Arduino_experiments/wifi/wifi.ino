#define BLYNK_PRINT Serial

#include "WiFi.h"
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char ssid[] = "Universe2";
char pass[] =  "Karakum!!!";
char auth[] = "7iSceGB4_iZkWKh3zxibWyFLzHh8nzvO";

 
void setup()
{
  Serial.begin(115200);
/*
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
*/
  Blynk.begin(auth, ssid, pass);


}

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

void loop()
{
  Blynk.run();  
}
