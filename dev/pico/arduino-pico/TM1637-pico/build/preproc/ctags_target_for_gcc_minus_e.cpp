# 1 "/home/alexey/dev/pico/arduino-pico/TM1637-pico/TM1637-pico.ino"
# 2 "/home/alexey/dev/pico/arduino-pico/TM1637-pico/TM1637-pico.ino" 2
# 3 "/home/alexey/dev/pico/arduino-pico/TM1637-pico/TM1637-pico.ino" 2
# 4 "/home/alexey/dev/pico/arduino-pico/TM1637-pico/TM1637-pico.ino" 2
# 5 "/home/alexey/dev/pico/arduino-pico/TM1637-pico/TM1637-pico.ino" 2




// Demo a clock, by default there will be a colon between the numbers.
int seconds = 0;
int minutes = 0;

void setup()
{
    TM1637_init(20, 19);
    TM1637_clear();

    TM1637_set_brightness(7); // max value, default is 0

    TM1637_display_word("dEMO", true);
    sleep_ms(2000);

    TM1637_put_4_bytes(1, 0x4f5b06); // raw bytes for 123 
    sleep_ms(1000);
    TM1637_set_brightness(0); // brightness is not updated automatically, 
    TM1637_put_4_bytes(1, 0x4f5b06); // something new needs to be displayed.
    sleep_ms(1000);

    TM1637_clear();
    sleep_ms(500);

    printf("DEMO\n");
    // Count down from 150 to -50
    int count = 150;
    TM1637_display(count, false);
    sleep_ms(500);
    while(count >= -50) {
      TM1637_display(count, false);
      count--;
      // The display can not update too often. So even though there is no
      // sleep, this will take a couple of moments.
    }

    sleep_ms(1000);
    TM1637_clear();
    sleep_ms(500);

    TM1637_display_both(minutes, seconds, true);

    while(true) {
        sleep_ms(1000);
        seconds++;
        if (seconds == 60) {
          seconds = 0;
          minutes++;
          TM1637_display_both(minutes, seconds, true);
        } else {
          TM1637_display_right(seconds, true);
        }
    }
}

void loop() {

}
