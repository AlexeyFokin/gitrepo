void setup() {
  // Configure asynchronous clock source
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC2_TC3_Val;    // select TC3 peripheral channel
  GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_GEN_GCLK0;        // select source GCLK_GEN[0]
  GCLK->CLKCTRL.bit.CLKEN = 1;            // enable TC3 generic clock
  while (GCLK->STATUS.bit.SYNCBUSY);



  // Enable and configure generic clock generator 6
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                      GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                      GCLK_GENCTRL_SRC_DFLL48M |  // Select 48MHz as source
                      GCLK_GENCTRL_ID(3) |         // Select GCLK4
                      GCLK_GENCTRL_OE;
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Set clock divider of 1 to generic clock generator 6
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(64) |         // Divide 48 MHz by 1
                     GCLK_GENDIV_ID(3);           // Apply to GCLK4 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  
  // Enable GCLK4
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                      GCLK_CLKCTRL_GEN_GCLK3;
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization




  
  // Configure synchronous bus clock
  PM->APBCSEL.bit.APBCDIV = 0;            // no prescaler
  PM->APBCMASK.bit.TC3_ = 1;                // enable TC3 interface
  
  // Configure Count Mode (16-bit)
  TC3->COUNT8.CTRLA.bit.MODE = TC_CTRLA_MODE_COUNT8_Val;
  
  // Configure Prescaler for divide by 2 (500kHz clock to COUNT)
  TC3->COUNT8.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV1_Val;
  
  // Configure TC3 Compare Mode for compare channel 0
  TC3->COUNT8.CTRLA.bit.WAVEGEN = TC_CTRLA_WAVEGEN_NFRQ_Val;         
  
  // Initialize compare value for 100mS @ 500kHz
  TC3->COUNT8.CC[0].reg = 200;
  while(TC3->COUNT8.STATUS.bit.SYNCBUSY == 1);
  TC3->COUNT8.CC[1].reg = 100;
  while(TC3->COUNT8.STATUS.bit.SYNCBUSY == 1);

  
  TC3->COUNT8.PER.reg = 31;
  while(TC3->COUNT8.STATUS.bit.SYNCBUSY == 1);
  
  // Enable TC3 compare mode interrupt generation
  //TC3->COUNT8.INTENSET.bit.MC0 = 0x1;    // Enable match interrupts on compare channel 0 
  
  TC3->COUNT8.CTRLA.bit.ENABLE = 1;
  while(TC3->COUNT8.STATUS.bit.SYNCBUSY == 1);


  // Configure PA19 (D10 on Arduino Zero) to be output
  PORT->Group[PORTA].DIRSET.reg = PORT_PA19;      // Set pin as output
  //PORT->Group[PORTA].DIRSET.reg = PORT_PA22;      // Set pin as output
  //PORT->Group[PORTA].OUTSET.reg = PORT_PA22; 


  // Enable the port multiplexer for PA19
  PORT->Group[PORTA].PINCFG[19].reg |= PORT_PINCFG_PMUXEN;
  PORT->Group[PORTA].PINCFG[17].reg |= PORT_PINCFG_PMUXEN;

  // Connect TCC0 timer to PA18. Function F is TCC0/WO[2] for PA18.
  // Odd pin num (2*n + 1): use PMUXO
  // Even pin num (2*n): use PMUXE
  PORT->Group[PORTA].PMUX[19/2].reg = PORT_PMUX_PMUXO_E;
  PORT->Group[PORTA].PMUX[17/2].reg = PORT_PMUX_PMUXO_H;
  

  /* Set TC3 Interrupt Priority to Level 3 */
  //NVIC_SetPriority(TC3_IRQn, 3);

  /* Enable TC3 NVIC Interrupt Line */
  //NVIC_EnableIRQ(TC3_IRQn);

  /* Enable Interrupts */
  //void __enable_irq(void);

  pinMode(A0, OUTPUT);
  //Serial.begin(115200);
}

void TC3_Handler(void){

  // Interrupt processing code
  //PORT->Group[PORTA].OUTTGL.reg = PORT_PA02;

  
  // Acknowledge the interrupt (clear MC0 interrupt flag to re-arm)
  TC3->COUNT8.INTFLAG.reg |= 0b00010000;

}

int sensorValue = 0;
void loop() {
//  digitalWrite(A0, HIGH);
//  digitalWrite(A0, LOW);
  //PORT->Group[PORTA].OUTTGL.reg = PORT_PA17; 
  
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(A2);
  sensorValue = sensorValue < 5 ? 5: sensorValue;

  TC3->COUNT8.CC[1].reg = sensorValue>>5;//<<6;
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY == 1);

  TC3->COUNT8.CC[0].reg = 15;//sensorValue<<6;
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY == 1);

    
  //Serial.println(sensorValue);
  delay(100);
  
}
