/*
 * Parth Sarthi Sharma (pss242@cornell.edu)
 * Code based on examples from Raspberry Pi Foundation.
 * The code initializes a PIO state machine on a given
 * GPIO and generates a square wave.
 */

#include "pico/stdlib.h" //Standard library for Pico
#include "hardware/pio.h" //The hardware PIO library
#include "PIOMaxSqaureWave.pio.h" //The pio header created after compilation

#define PIN 20 //Define the GPIO Pin

PIO pio = pio0; //Identifier for the first (PIO 0) hardware PIO instance

void setup() { //The main function

    uint offset = pio_add_program(pio, &PIOMaxSqaureWave_program); //Attempt to load the program
    uint sm = pio_claim_unused_sm(pio, true); //Claim a free state machine on a PIO instance
    PIOMaxSqaureWave_program_init(pio, sm, offset, PIN, 1.0f); //Initialize the program

}
bool a=true;
void loop() {}

