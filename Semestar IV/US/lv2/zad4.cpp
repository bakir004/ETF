#include "mbed.h"

BusOut leds(p5, p6, p7, p8, p9, p10, p11, p12);
DigitalOut polling(p13)

int main() {
    int running = 1;
    bool active = false;
    bool on = false;
    while (1) {
        
        if(active) {
            running <<= 1;
            if(running == 0b100000000) {
                running = 1;
                active = false;
            }
            if(active)
                leds = running;
        } else {
            polling = !polling;
            wait_ms(500);
        }
        
        

        wait_ms(100);
    }
}
