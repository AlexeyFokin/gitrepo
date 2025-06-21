# 1 "/home/alexey/dev/pico/arduino-pico/pio/squarewave/squarewave.ino"
/*
 * Parth Sarthi Sharma (pss242@cornell.edu)
 * Code based on examples from Raspberry Pi Foundation.
 * The code initializes a PIO state machine on a given
 * GPIO and generates a square wave.
 */

# 9 "/home/alexey/dev/pico/arduino-pico/pio/squarewave/squarewave.ino" 2
# 10 "/home/alexey/dev/pico/arduino-pico/pio/squarewave/squarewave.ino" 2
# 11 "/home/alexey/dev/pico/arduino-pico/pio/squarewave/squarewave.ino" 2



void setup() { //The main function
    PIO pio = ((pio_hw_t *)0x50200000u); //Identifier for the first (PIO 0) hardware PIO instance
    uint offset = pio_add_program(pio, &PIOMaxSqaureWave_program); //Attempt to load the program
    uint sm = pio_claim_unused_sm(pio, true); //Claim a free state machine on a PIO instance
    PIOMaxSqaureWave_program_init(pio, sm, offset, 20 /*Define the GPIO Pin*/, 1.0f); //Initialize the program

}

void loop() {}
