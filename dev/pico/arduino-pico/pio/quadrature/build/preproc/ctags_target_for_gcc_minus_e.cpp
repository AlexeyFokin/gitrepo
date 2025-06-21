# 1 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino"
//  SPDX-FileCopyrightText: 
//2022 Jamon Terrell <github@jamonterrell.com>
//2024 Arda Alıcı <ardayaozel@hotmail.com>
//  SPDX-License-Identifier: MIT

# 7 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2
# 8 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2
# 9 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2
# 10 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2
# 11 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2

# 13 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2


# 16 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2
# 17 "/home/alexey/dev/pico/arduino-pico/pio/quadrature/quadrature.ino" 2







PIO pio = ((pio_hw_t *)0x50200000u);
uint sm = pio_claim_unused_sm(pio, true);

TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h

byte font_size = 6;

VL53L0X sensor;

void setup() {
    Serial.begin(9600);
    Wire.setSDA(12);
    Wire.setSCL(13);
    Wire.begin();

    //delay(3000);

    sensor.setTimeout(500);
    if (!sensor.init())
    {
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {}
    }

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensor.startContinuous();

    uint offset = pio_add_program(pio, &quadratureB_program);

    quadratureB_program_init(pio, sm, offset, 21, 23, 1000000.f);

    tft.init();

    tft.setRotation(1);
    // First we test them with a background colour set
    //tft.setTextSize(1);
    tft.fillScreen(0x0000 /*   0,   0,   0 */);
    tft.setTextColor(0x867D /* 135, 206, 235 */, 0x0000 /*   0,   0,   0 */);

    tft.setTextDatum(4 /* Centre centre, same as above*/);
    int padding = tft.textWidth("-999", font_size); // get the width of the text in pixels
    tft.setTextPadding(padding);

    pinMode(22, OUTPUT);
    digitalWrite(22, HIGH); // power on LCD
}

void loop() {
    sleep_ms(100);
    pio_sm_exec_wait_blocking(pio, sm, pio_encode_in(pio_x, 32));
    int x = pio_sm_get_blocking(pio, sm);


    printf("%d\n", x);

    int r = sensor.readRangeContinuousMillimeters();

    tft.drawNumber(x, 100, 50, font_size);
    tft.setTextColor(0xFEA0 /* 255, 215,   0 */, 0x0000 /*   0,   0,   0 */);
    tft.drawNumber(r, 100, 100, font_size);

    tft.drawLine(0, 130, 239, 130, 0x0000 /*   0,   0,   0 */);
    tft.drawLine(0, 131, 239, 131, 0x0000 /*   0,   0,   0 */);
    tft.drawLine(0, 130, r/2, 130, 0xFDA0 /* 255, 180,   0 */);
    tft.drawLine(0, 131, r/2, 131, 0xFDA0 /* 255, 180,   0 */);


    //135 240

    Serial.print(sensor.readRangeContinuousMillimeters());
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    Serial.println();
}
