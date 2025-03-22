#include "mbed.h"

DigitalOut leds[] = {DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3), DigitalOut(LED4)};

int main() {
    int i = 0;
    while(1) {
        leds[i] = 1;
        wait(1);
        leds[i] = 0;
        wait(1);
        i = (i + 1) % 4;
    }
}
