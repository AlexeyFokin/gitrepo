#undef A0
#define A0 13

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(PIN_LED_RXL, OUTPUT);  //LED_BUILTIN  PIN_LED_RXL 
  //pinMode(PIN_LED_TXL, OUTPUT);
  pinMode(13, OUTPUT);

  // Change GCLK_MAIN frequency divisor
    //GCLK->GENDIV.reg = GCLK_GENDIV_ID(0) | GCLK_GENDIV_DIV(16);
    //while (GCLK->STATUS.bit.SYNCBUSY != 0) {}
}

// the loop function runs over and over again forever
long a= 10;
void loop() {
  //digitalWrite(PIN_LED_RXL, HIGH);   // turn the LED on (HIGH is the voltage level)
  //digitalWrite(PIN_LED_TXL, LOW);
  digitalWrite(13, HIGH);
  //delay(1000);                       // wait for a second
  a = del(2);
  
  Serial.println(a);
  //digitalWrite(PIN_LED_RXL, LOW);    // turn the LED off by making the voltage LOW
  //digitalWrite(PIN_LED_TXL, HIGH);
  digitalWrite(13, LOW);
  //delay(1000);                       // wait for a second
  a = del(3);
   Serial.println(a);


  *((uint8_t*) &GCLK->GENCTRL.reg) = 0;
  while (GCLK->STATUS.bit.SYNCBUSY == 1) {}
  int t = GCLK->GENCTRL.reg;
  Serial.println(t);
  
}

long del(int r)
{
  long i = 0;
  long a = 0;
  for(i=0; i<3500000; i++) {
    a = i+a+r;
  }
  return a;
}
