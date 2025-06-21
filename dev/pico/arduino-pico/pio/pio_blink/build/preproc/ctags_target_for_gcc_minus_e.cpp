# 1 "/home/alexey/dev/pico/arduino-pico/pio_blink/pio_blink.ino"
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

# 8 "/home/alexey/dev/pico/arduino-pico/pio_blink/pio_blink.ino" 2

# 10 "/home/alexey/dev/pico/arduino-pico/pio_blink/pio_blink.ino" 2
# 11 "/home/alexey/dev/pico/arduino-pico/pio_blink/pio_blink.ino" 2
# 12 "/home/alexey/dev/pico/arduino-pico/pio_blink/pio_blink.ino" 2
# 13 "/home/alexey/dev/pico/arduino-pico/pio_blink/pio_blink.ino" 2

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq);

void setup() {
    // setup_default_uart();

    // todo get free sm
    PIO pio = ((pio_hw_t *)0x50200000u);
    uint offset = pio_add_program(pio, &blink_program);
    delay(4000);
    printf("Loaded program at %d\n", offset);

    blink_pin_forever(pio, 0, offset, 0, 3);
    blink_pin_forever(pio, 1, offset, 6, 4);
    blink_pin_forever(pio, 2, offset, 25, 1);
}

void loop() {}

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
}
