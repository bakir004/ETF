#include "mbed.h"

BusOut leds(p5, p6, p7, p8, p9, p10, p11, p12);

int main() {
    int running = 1;
    while (1) {
        running <<= 1;
        if(running == 0b100000000)
            running = 1;
        leds = running;

        wait_ms(100);
    }
}
