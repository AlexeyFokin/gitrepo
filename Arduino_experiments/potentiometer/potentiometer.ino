
 /*
  ESP32 Analog Read
  Read potentiometer voltage at different inputs

  Written by Ahmad Shamshiri at 20:55
  on Sunday Sep 01, 2019 in Ajax, Ontario, Canada

  Watch Video instruction for this sketch: 
https://youtu.be/UXKu2Xtwj7c
 
*/

const int potPin = 10;// Analog pin

int potValue;// do not change
float voltage =0;// do not change


void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
}


void loop() {
  // read the input on analog pin potPin:
  potValue = analogRead(potPin);
  voltage = (3.3/4095.0) * potValue;
  Serial.print("potValue:");
  Serial.print(potValue);
   
  Serial.print(" Voltage:");
  Serial.print(voltage);
  Serial.println("V");  
  delay(50); // delay in between reads for stability
}
