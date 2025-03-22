#include "mbed.h"

BusOut leds(p5, p6, p7, p8, p9, p10, p11, p12);

int main() {
    int count = 0;
    int add = 1;
    while (1) {
        count += add;
        leds = count;

        wait_ms(500);
    }
}
