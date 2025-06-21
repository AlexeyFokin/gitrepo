int ledPin = 1;
int val;
void setup() {
}
void loop() {
  for (val = 0; val <= 1024; val++) {         // 100 -> mean the frequency is 941.1Hz
  pwm(ledPin, 20000, val);
    delay(1);
  }
  for (val = 1024; val >= 0; val--) {
    pwm(ledPin, 20000, val);
    delay(1);
  }
}