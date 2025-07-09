from machine import Pin
from time import sleep

pin = Pin(4, Pin.OUT)

while True:
    pin.toggle()
    sleep(0.5)