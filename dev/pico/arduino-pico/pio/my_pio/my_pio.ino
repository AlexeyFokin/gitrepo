/**
 * PIO experimenting
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "my_pio.pio.h"


// Need to connect a wire from GPIO2 -> GPIO3
const uint pin_tx = 18;

const uint start = 13;


PIO pio = pio0;
uint sm_tx = 1;


void setup() {

    //gpio_init(13);
    //gpio_set_dir(13, GPIO_OUT);
    //gpio_put(13, 0);

    // while (true) {
    //     gpio_put(18, 1);
    //     sleep_ms(250);
    //     gpio_put(18, 0);
    //     sleep_ms(250);
    // }

    uint offset_tx = pio_add_program(pio, &my_pio_experiment_program);
    printf("Transmit program loaded at %d\n", offset_tx);

    my_pio_experiment_program_init(pio, sm_tx, offset_tx, pin_tx, start, 100.f);
    pio_sm_set_enabled(pio, sm_tx, true);
}

void loop() {
    while(true) {

        //gpio_put(13, 1);
        //delay(0.01);
        //gpio_put(13, 0);
        pio_sm_put_blocking(pio, sm_tx, 0xaaaaaaaa);
        pio_sm_put_blocking(pio, sm_tx, 0xaaaa5555);
        //pio_sm_put_blocking(pio, sm_tx, 0x0000010);
        //pio_sm_put_blocking(pio, sm_tx, 0x0000010);
        //pio_sm_put_blocking(pio, sm_tx, 0x0000010);
        //pio_sm_put_blocking(pio, sm_tx, 0x0000010);
        //pio_sm_put_blocking(pio, sm_tx, 0x0000010);
        //pio_sm_put_blocking(pio, sm_tx, 0x0001111);

    //    pio_sm_put_blocking(pio, sm_tx, 0x0ff0a55a);
    //    pio_sm_put_blocking(pio, sm_tx, 0x12345678);
    }

}
