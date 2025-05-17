#include "mbed.h"
#include "lpc1114etf.h"

BusOut prikaz1(LED3, LED2, LED1, LED0);
BusOut prikaz2(LED7, LED6, LED5, LED4);
DigitalOut enable(LED_ACT);
InterruptIn taster(Taster_1);

Ticker ticker;
Timer timer;

const float T = 0.2f; 
const int debounce = 50; 

int brojac1 = 0;
int brojac2 = 0;

void br1() {
  brojac1 = (brojac1 + 1) % 16;
  prikaz1 = brojac1;
}

void br2() {
  if (timer.read_ms() < debounce) return; 

  brojac2 = (brojac2 + 1) % 16;
  prikaz2 = brojac2;

  timer.reset(); 
}

int main() {
  enable = 0;

  prikaz1 = brojac1;
  prikaz2 = brojac2;

  timer.start(); 
  timer.reset();

  ticker.attach(&br1, T);
  taster.rise(&br2);

  while (1) { }
}
