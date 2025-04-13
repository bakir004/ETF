from machine import Pin, ADC
import time

led_pins = [Pin(4, Pin.OUT),Pin(5, Pin.OUT),Pin(6, Pin.OUT),
            Pin(7, Pin.OUT),Pin(8, Pin.OUT),Pin(9, Pin.OUT),
            Pin(10, Pin.OUT),Pin(11, Pin.OUT),] 

pot = ADC(2)

def get_delay():
    adc_value = pot.read_u16() 
delay = 0.1 + (adc_value / 65535) * (2.0 - 0.1)
return delay

while True:
    for led in led_pins:
        led.value(1)
time.sleep(get_delay())

for led in reversed(led_pins):
    led.value(0)
time.sleep(get_delay())
