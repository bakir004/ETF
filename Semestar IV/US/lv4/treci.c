#include "mbed.h"
#include "lpc1114etf.h"

AnalogIn potentiometer(AD1);

DigitalOut leds[] = {
    DigitalOut(LED0), DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3),
    DigitalOut(LED4), DigitalOut(LED5), DigitalOut(LED6), DigitalOut(LED7)
};

DigitalOut e(LED_ACT);

void update_leds(float voltage) {
    int num_leds = (voltage / 3.3f) * 8;
    for (int i = 0; i < 8; i++)
        leds[i] = (i < num_leds) ? 1 : 0; 
}

int main() {
    e=0;
    while (true) {
        float voltage = potentiometer.read() * 3.3f; 
        wait_us(500000);
        update_leds(voltage);
    }
}
