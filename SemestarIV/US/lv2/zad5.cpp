#include "mbed.h"
#include <math.h>

#define eps 0.00001

DigitalOut led(p5);

int main() {
    double T = 0.1;
    double timeOn = T;
    double timeOff = T;
    double onAdder = T/20;
    double offAdder = -0.8*T/20;
    while (1) {
        led = 1;
        wait_ms(timeOn*1000);
        led = 0;
        wait_ms(timeOff*1000);
        timeOn += onAdder;
        timeOff += offAdder;
        if(fabs(timeOff - 0.2*T) <= eps || fabs(timeOff - 2*T) <= eps) {
            onAdder *= -1;
            offAdder *= -1;
        }
        printf("On: %f\n", timeOn);
        printf("Off: %f\n", timeOff);
    }
}
