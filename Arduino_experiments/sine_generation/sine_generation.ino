
/*
  Seeeduino XIAO Analog Output Demo
  xiao-analog-out-test.ino
  Displays operation of XIAO D/A converter
  Produces a sine wave on Analog output
  Reads result on analog input
  Results displayed on Serial Plotter
 
  Adapted from Seeeduino sketch on https://wiki.seeedstudio.com/Seeeduino-XIAO
 
  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/
 
// Define Analog Out pin
#define DAC_PIN A0
 
// Define Analog In pin
#define ADC_PIN A4
 
// Float output to modify for sine wave
float outval = 0;
// Increment
float increment = 0.02;
 
 
void setup()
{
  // Set analog output resolution to 10-bits
  analogWriteResolution(10);
 
  // Set analog input resolution to 12-bits
  analogReadResolution(12);
 
  // Setup Serial Port
  SerialUSB.begin(9600);
}
 
void loop()
{
  // Generate a voltage value between 0 and 1023.
  // Offset by 511.5 (half-point)
  int dacVoltage = (int)(511.5 + 511.5 * outval);
 
  // Increment
  outval += increment;
  if (outval>=0.98 || outval <=-0.98)
  {
    increment *= -1;
  }
 
  // Output to DAC port
  analogWrite(DAC_PIN, (dacVoltage>>6)<<6);
 
  // Read DAC output on ADC input
  float voltage = analogRead(ADC_PIN) * 3.3 / 4096.0;
 
  // Print to serial plotter
  SerialUSB.println(voltage);
 
  // 1ms delay (change to change sine wave frequency)
  delay(1);
}
