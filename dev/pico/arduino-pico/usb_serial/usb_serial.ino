#include <stdio.h>
#include "pico/stdlib.h"

void setup() {
      //Serial.begin(9600);
    gpio_init(18);
    gpio_set_dir(18, GPIO_OUT);
    gpio_put(18, 0);
}

void loop() {
    // stdio_init_all();
    while (true) {
        gpio_put(18, 1);
    //    sleep_ms(250);
        gpio_put(18, 0);
        printf("@@@@@\n");
        //printf("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
        //sleep_ms(1000);
    }
}
