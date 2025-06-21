//  SPDX-FileCopyrightText: 
//2022 Jamon Terrell <github@jamonterrell.com>
//2024 Arda Alıcı <ardayaozel@hotmail.com>
//  SPDX-License-Identifier: MIT

#include <stdio.h>
#include "pico.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "quadrature.pio.h"

#include <TFT_eSPI.h>
#include <SPI.h>

#include <Wire.h>
#include <VL53L0X.h>

#define QUADRATURE_A_PIN 21
#define QUADRATURE_B_PIN 23

#define PWR_ON 22


PIO pio = pio0;
uint sm = pio_claim_unused_sm(pio, true);

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

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

    quadratureB_program_init(pio, sm, offset, QUADRATURE_A_PIN, QUADRATURE_B_PIN, 1000000.f);

    tft.init();
    
    tft.setRotation(1);
    // First we test them with a background colour set
    //tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);

    tft.setTextDatum(CC_DATUM);
    int padding = tft.textWidth("-999", font_size); // get the width of the text in pixels
    tft.setTextPadding(padding);

    pinMode(PWR_ON, OUTPUT);
    digitalWrite(PWR_ON, HIGH); // power on LCD
}
 
void loop() {
    sleep_ms(100);
    pio_sm_exec_wait_blocking(pio, sm, pio_encode_in(pio_x, 32));
    int x = pio_sm_get_blocking(pio, sm);


    printf("%d\n", x);

    int r = sensor.readRangeContinuousMillimeters();

    tft.drawNumber(x, 100, 50, font_size);
    tft.setTextColor(TFT_GOLD, TFT_BLACK);
    tft.drawNumber(r, 100, 100, font_size);

    tft.drawLine(0, 130, 239, 130, TFT_BLACK);
    tft.drawLine(0, 131, 239, 131, TFT_BLACK);
    tft.drawLine(0, 130, r/2, 130, TFT_ORANGE);
    tft.drawLine(0, 131, r/2, 131, TFT_ORANGE);


    //135 240

    Serial.print(sensor.readRangeContinuousMillimeters());
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    Serial.println();
}
