
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(115200);
  Serial.println("setup");
}

int buttonState = 0;

// the loop function runs over and over again forever
void loop() {
  digitalWrite(6, HIGH);   // turn the LED off (HIGH is the voltage level)
  digitalWrite(6, HIGH);
  delay(250);                       // wait for a long time while the LED remains off
  digitalWrite(7, LOW);    // turn the LED on by making the voltage LOW
  digitalWrite(7, LOW);
  delay(250); 
  // wait for a short time to briefly light the LED


 
  
}
