#include "mbed.h"
#include "lpc1114etf.h"

AnalogIn potenciometar(dp9);
DigitalOut e(LED_ACT);

PwmOut led(LED1);

int main() {
    e = 0;
    led.period(0.02f); 

    while (true) {
        float vrijednost = potenciometar.read();

        led.write(vrijednost); 

        wait_us(10000); 
    }
}
