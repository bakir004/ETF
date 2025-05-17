#include "mbed.h"
#include "lpc1114etf.h"

DigitalOut cetvrtke(dp18);
DigitalIn impulsi(dp9);
BusOut displej(LED4, LED5, LED6, LED7, LED0, LED1, LED2, LED3);
const int T_us = 2000;
int count = 0;

int main() {
    impulsi = 0;
    int prosli = 0;

    while (true) {
        cetvrtke = 1;
        wait_us(T_us / 2);
        cetvrtke = 0;
        wait_us(T_us / 2);

        int trenutni = impulsi.read();
        if (prosli == 0 && trenutni == 1)
            count = (count + 1) % 100;
        prosli = trenutni;

        displej = (count % 10) << 4 | (count / 10);
    }
}
