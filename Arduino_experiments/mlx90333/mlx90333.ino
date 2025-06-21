#include <SPI.h>
#include <stdint.h>

//*******Arduino UNO SPI pins********
#define SS 0  //brown
#define SCK 8  //blue
#define MOSI 9  //SDI MOSI green
#define MISO 10  //SDO MISO yellow

byte dataBuffer[8];

void setup() {
  Serial.begin(9600); 
  SPI.begin();
  pinMode(SS, OUTPUT);   //
  pinMode(SCK, OUTPUT);  // All necessary though SPI.begin should take care of this
  pinMode(MOSI, OUTPUT); //
  pinMode(MISO, INPUT);  //
}

void loop() {
  delay(200); //MLX90333 startup takes 16ms
  SPI.beginTransaction(SPISettings(20000, MSBFIRST, SPI_MODE1)); //320000 is about max
  delay(5);
  int j;
  for (j=0; j<10; j++){
    digitalWrite(SS, LOW);
    delay(200);  //Short delay necessary here
    int i;
    for (i=0; i<8; i++){ 
      dataBuffer[i] = SPI.transfer(255);  // Must transfer 1's, 0's don't work
      }
    digitalWrite(SS, HIGH);
    SPI.endTransaction();
    Serial.print(dataBuffer[2],8); //Print 3rd byte, MSB for Alpha
    Serial.print(" ,");
    Serial.print(dataBuffer[4],8); //Print 5th byte, MSB for Beta
    Serial.println(" ,");
    }
  }
