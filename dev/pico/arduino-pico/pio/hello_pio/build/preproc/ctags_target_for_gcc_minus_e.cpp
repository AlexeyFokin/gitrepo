# 1 "/home/alexey/dev/pico/arduino-pico/hello_pio/hello_pio.ino"
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



# 10 "/home/alexey/dev/pico/arduino-pico/hello_pio/hello_pio.ino" 2
# 11 "/home/alexey/dev/pico/arduino-pico/hello_pio/hello_pio.ino" 2
// Our assembled program:
# 13 "/home/alexey/dev/pico/arduino-pico/hello_pio/hello_pio.ino" 2

// Choose which PIO instance to use (there are two instances)
PIO pio = ((pio_hw_t *)0x50200000u);

// Find a free state machine on our chosen PIO (erroring if there are
// none). Configure it to run our program, and start it, using the
// helper function we included in our .pio file.
uint sm = pio_claim_unused_sm(pio, true);

void setup() {





    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(pio, &hello_program);


    hello_program_init(pio, sm, offset, 25);

    // The state machine is now running. Any value we push to its TX FIFO will
    // appear on the LED pin.
}

void loop() {

    while (true) {
        // Blink
        pio_sm_put_blocking(pio, sm, 1);
        sleep_ms(500);
        // Blonk
        pio_sm_put_blocking(pio, sm, 0);
        sleep_ms(500);
    }

}
