void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  //printChipId();
  volatile uint32_t val1;
  for(int gen=0; gen<8; gen++) { 
    clock_get_frequency(gen);
  }
  
  
  delay(3000);
}

void printChipId() {
  volatile uint32_t val1, val2, val3, val4;
  volatile uint32_t *ptr1 = (volatile uint32_t *)0x0080A00C;
  val1 = *ptr1;
  volatile uint32_t *ptr = (volatile uint32_t *)0x0080A040;
  val2 = *ptr;
  ptr++;
  val3 = *ptr;
  ptr++;
  val4 = *ptr;

  Serial.print("chip id: 0x");
  char buf[33];
  sprintf(buf, "%8x%8x%8x%8x", val1, val2, val3, val4);
  Serial.println(buf);

  
}

void clock_get_frequency(uint8_t gen) {

        *((uint8_t*) &GCLK->GENCTRL.reg) = gen;
        *((uint8_t*) &GCLK->GENDIV.reg) = gen;
        while (GCLK->STATUS.bit.SYNCBUSY == 1) {}

        uint8_t src = GCLK->GENCTRL.bit.SRC;
        uint32_t div;
        if (GCLK->GENCTRL.bit.DIVSEL) {
            div = 1 << (GCLK->GENDIV.bit.DIV + 1);
        } else {
            div = GCLK->GENDIV.bit.DIV;
            if (!div)
                div = 1;
        }
        uint8_t genen = GCLK->GENCTRL.bit.GENEN;
        Serial.println("Generator n=" + String(gen) + ", source: " + String(src) + ", divisor: " + div + ", GENEN=" + String(genen));
        
        return;
}
