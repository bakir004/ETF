from machine import Pin
from time import sleep_ms
ledovi = [Pin(i, Pin.OUT) for i in range(4, 12)]

enc = Pin(0, Pin.IN, Pin.PULL_UP)
dt = Pin(1, Pin.IN, Pin.PULL_UP)
res = Pin(2, Pin.IN, Pin.PULL_UP)

brojac = 0

def prikazi(broj):
    for i in range(8):
        ledovi[i].value((broj >> i) & 1)

def enkoder():
    global brojac
    if dt.value() != enc.value():  
        brojac = (brojac + 1) % 256
    else:
        brojac = (brojac - 1) % 256
    prikazi(brojac)

def reset():
    sleep_ms(50)  
    if res.value() == 0:
        brojac = 0
        prikazi(brojac)

enc.irq(trigger=Pin.IRQ_RISING, handler=enkoder)
res.irq(trigger=Pin.IRQ_RISING, handler=reset)

prikazi(brojac)

while True:
    pass
