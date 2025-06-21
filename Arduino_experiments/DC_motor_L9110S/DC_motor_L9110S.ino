// L9110S DC motor driver
// PWM speed and direction


#include <Wire.h>
#include <AS5600.h>

const int A1 = 17;
const int A2 = 22;

int speed = 255;
int potpin = A6;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

AMS_5600 ams5600;

int ang, lang = 0;

const int array_l = 1000;
word angles[array_l];
int idx = 0;

void setup() {
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  Serial.begin(115200);

  Wire.begin(19, 23);
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
  if(ams5600.detectMagnet() == 0 ){
    while(1){
        if(ams5600.detectMagnet() == 1 ){
            Serial.print("Current Magnitude: ");
            Serial.println(ams5600.getMagnitude());
            break;
        }
        else{
            Serial.println("Can not detect magnet");
        }
        delay(1000);
    }
  }
  
}

float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
  float retVal = newAngle * 0.087;
  ang = retVal;
  return ang;
}

void loop() {

    val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
    
    int speed = map(val, 0, 4095, 0, 255);   // scale it to use it with the servo (value between 0 and 180)
    //Serial.println(speed);
    analogWrite(A2, 0);
    analogWrite(A1, speed);
  
  while(1)
  {    
  
    //Serial.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));
    angles[idx++] = ams5600.getRawAngle();
    if(idx == array_l)
    {
      idx = 0;
      Serial.println("----");
      for (int i = 0; i < array_l; i++)
      {
        Serial.println(String(convertRawAngleToDegrees(angles[i]),DEC));
      }
      Serial.println("----");
    }
    //delay(10);

  }
}
