from machine import Pin, PWM, UART, ADC
import time

# UART za Bluetooth
uart = UART(0, baudrate=9600)

# Motor smjer
lijevi_fwd = Pin(7, Pin.OUT)
lijevi_fwd.on()

desni_fwd = Pin(17, Pin.OUT)
desni_fwd.on()

# PWM izlazi
pwm_lijevi = PWM(Pin(6))
pwm_desni = PWM(Pin(20))
pwm_lijevi.freq(1000)
pwm_desni.freq(1000)

# ADC za Sharp senzor (na GPIO26 / ADC0)
senzor = ADC(26)

# Funkcija za postavljanje PWM dužnosti
def set_pwm(posto):
    duty = int(65535 * posto / 100)
    pwm_lijevi.duty_u16(duty)
    pwm_desni.duty_u16(duty)

# Početno stanje
trenutna_brzina = 0
komanda_pwm = 0  # default dok ne stigne "a" ili "b"

print("Start: čekam komande i čitam senzor blizine.")

while True:
    # 1. Provjera senzora blizine
    adc_raw = senzor.read_u16()  # 0–65535
    napon = adc_raw * 3.3 / 65535  # u voltima

    prepreka = napon > 1.8  # prag detekcije (~ <30 cm)

    # 2. Obrada Bluetooth komande
    if uart.any():
        data = uart.read()
        if data:
            try:
                text = data.decode('utf-8').strip()
                print("Primljeno:", text)

                if text == "forward":
                    komanda_pwm = 100
                    print("▶️ Komanda: PWM 100%")
                elif text == "backward":
                    komanda_pwm = 0
                    print("⏹️ Komanda: STOP")
                else:
                    print("Nepoznata komanda.")

            except UnicodeError:
                print("Greška u dekodiranju teksta.")

    # 3. Primjena logike vožnje
    if prepreka:
        set_pwm(0)
        if trenutna_brzina != 0:
            print("🛑 Prepreka detektovana! Zaustavljanje.")
        trenutna_brzina = 0
    else:
        set_pwm(komanda_pwm)
        if trenutna_brzina != komanda_pwm:
            print(f"✅ PWM postavljen: {komanda_pwm}%")
        trenutna_brzina = komanda_pwm

    time.sleep(0.1)