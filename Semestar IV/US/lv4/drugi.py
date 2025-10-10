from ili934xnew import ILI9341, color565
from machine import Pin, SPI, ADC
from micropython import const
import os
import glcdfont
import tt14
import tt24
import tt32
import time

adc = ADC(Pin(28))

SCR_WIDTH = const(320)
SCR_HEIGHT = const(240)
SCR_ROT = const(2)
CENTER_Y = SCR_WIDTH // 2
CENTER_X = SCR_HEIGHT // 2

print(os.uname())

TFT_CLK_PIN = const(18)
TFT_MOSI_PIN = const(19)
TFT_MISO_PIN = const(16)
TFT_CS_PIN = const(17)
TFT_RST_PIN = const(20)
TFT_DC_PIN = const(15)

fonts = [glcdfont, tt14, tt24, tt32]

text = 'RPi Pico/ILI9341'
print(text)
print("Fontovi:")
for f in fonts:
    print(f.__name__)

spi = SPI(
        0,
        baudrate=62500000,
        miso=Pin(TFT_MISO_PIN),
        mosi=Pin(TFT_MOSI_PIN),
        sck=Pin(TFT_CLK_PIN)
        )

print(spi)

display = ILI9341(
        spi,
        cs=Pin(TFT_CS_PIN),
        dc=Pin(TFT_DC_PIN),
        rst=Pin(TFT_RST_PIN),
        w=SCR_WIDTH,
        h=SCR_HEIGHT,
        r=SCR_ROT
        )

display.set_font(tt14)
display.set_pos(240, 25)
display.rotation = 3
display.init()

display.set_pos(0, 200)
x, y = 20, 220
while x <= y:
    display.pixel(x, y, color565(0, 0, 0))
    y -= 1

x, y = 10, 200
while y >= x:
    display.pixel(x, y, color565(0, 0, 0))
    x += 1

display.set_pos(0, 25)
display.print('40')
display.set_pos(0, 110)
display.print('30')
display.set_pos(0, 195)
display.print('20')

time.sleep(2)

lm35_volt = ADC(28)

vrijeme = 0
while True:
    lm35_volt_mv = lm35_volt.read_u16() * 3300 / 65535
    temp = adc.read_u16()

    display.set_font(tt14)
    x = vrijeme + 20
    y = 10 + (temp * 330 / 65535 - 20) * (110 - 25) / 10

    display.pixel(round(x), round(y), color565(0, 0, 0))

    display.set_pos(250, 25)
    display.print(str(lm35_volt_mv))
    display.print(str(temp * 330 / 65535))
    display.print(str(vrijeme))

    display.set_pos(200, 25)
    display.print('Napon:')
    display.print('Temp:')
    display.print('Vrijeme:')

    time.sleep(0.1)
    vrijeme += 1

    if vrijeme > 200:
        break
