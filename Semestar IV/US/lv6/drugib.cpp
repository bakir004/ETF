#include "mbed.h"
#include "lpc1114etf.h"

DigitalOut cetvrtka(dp18);        
InterruptIn impuls(dp9);         
BusOut prikaz1(LED0, LED1, LED2, LED3); 
BusOut prikaz2(LED4, LED5, LED6, LED7); 
DigitalOut enable(LED_ACT);
Ticker t;    

int count = 0;

void toggle() { cetvrtka = !cetvrtka; }

void broji() {
  count = (count + 1) % 100; 

  int jedinice = count % 10;
  int desetice = count / 10;

  prikaz1 = jedinice; 
  prikaz2 = desetice;   
}

int main() {
  enable=0;
  cetvrtka = 0;

  impuls.rise(&broji); 

  t.attach_us(&toggle, 1000); 

  while (true) { }
}
