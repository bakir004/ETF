//ZADATAK 1
import time
from machine import Pin

# Definicija dugmića sa pull-up otpornicima
button1 = Pin(0, Pin.IN, Pin.PULL_DOWN)
button2 = Pin(1, Pin.IN, Pin.PULL_DOWN)
button3 = Pin(2, Pin.IN, Pin.PULL_DOWN)
button4 = Pin(3, Pin.IN, Pin.PULL_DOWN)

# Definicija segmenata za prikaz brojeva na 7-segmentnom displeju
numbers = {
    0: [0, 0, 0, 0, 0, 0, 1, 1],
    1: [1, 0, 0, 1, 1, 1, 1, 1],
    2: [0, 0, 1, 0, 0, 1, 0, 1],
    3: [0, 0, 0, 0, 1, 1, 0, 1],
    4: [1, 0, 0, 1, 1, 0, 0, 1],
    5: [0, 1, 0, 0, 1, 0, 0, 1],
    6: [0, 1, 0, 0, 0, 0, 0, 1],
    7: [0, 0, 0, 1, 1, 1, 1, 1],
    8: [0, 0, 0, 0, 0, 0, 0, 1],
    9: [0, 0, 0, 0, 1, 0, 0, 1]
}

# Definisanje izlaznih pinova za kontrolu displeja
digits = [Pin(7, Pin.OUT), Pin(6, Pin.OUT), Pin(5, Pin.OUT), Pin(4, Pin.OUT)]
segments = [Pin(i, Pin.OUT) for i in range(8, 16)]

# Inicijalizacija segmenata
for segment in segments:
    segment.value(1)

brojac = 0  # Inicijalizacija brojača

def postaviSegment(cifra):
    """Postavlja određenu cifru na 7-segmentnom displeju."""
    for i in range(8):
        segments[i].value(numbers[cifra][i])

prethodno = [False, False, False, False]
automatski = False
tt = 0

while True:
    # Provera tastera
    if button1.value() and not prethodno[0]:
        brojac += 1
    elif button2.value() and not prethodno[1]:
        brojac -= 1
        if brojac < 0:
            brojac = 0
    elif button3.value() and not prethodno[2]:
        brojac = 0
    elif button4.value() and not prethodno[3]:
        automatski = not automatski

    prethodno = [button1.value(), button2.value(), button3.value(), button4.value()]

    if automatski:
        tt += 1
        if tt == 100:
            tt = 0
            brojac += 1

    # Prikaz broja na displeju
    tmp = brojac
    for i in range(4):
        postaviSegment(tmp % 10)
        digits[i].value(0)
        time.sleep(0.0025)
        digits[i].value(1)
        tmp //= 10




//ZADATAK 2
from machine import Pin
from time import sleep

# Definicija pinova za segmente (A, B, C, D, E, F, G, DP)
SEGMENTS = [
    Pin(8, Pin.OUT),  # A
    Pin(9, Pin.OUT),  # B
    Pin(10, Pin.OUT),  # C
    Pin(11, Pin.OUT),  # D
    Pin(12, Pin.OUT),  # E
    Pin(13, Pin.OUT),  # F
    Pin(14, Pin.OUT),  # G
    Pin(15, Pin.OUT)   # DP (decimalna tačka)
]

# Definicija pinova za cifre (DIG1, DIG2, DIG3, DIG4)
DIGITS = [
    Pin(4, Pin.OUT),  # D1
    Pin(5, Pin.OUT),  # D2
    Pin(6, Pin.OUT),  # D3
    Pin(7, Pin.OUT)   # D4
]

# Funkcija za aktiviranje određene cifre
def activate_digit(digit_index):
    for i in range(4):  # Broj cifara je 4
        DIGITS[i].value(1)  # Isključi sve cifre (0 znači isključen za anodni displej)
    DIGITS[digit_index].value(0)  # Uključi samo jednu cifru (1 znači uključen za anodni displej)

# Funkcija za deaktivaciju svih cifara
def deactivate_all_digits():
    for digit in DIGITS:
        digit.value(1)  # Isključi sve cifre

# Funkcija za uključivanje segmenata jedan po jedan na određenoj cifri, dok se prethodni ne ugase
def display_segments_on_digit(digit_index):
    deactivate_all_segments()  # Osiguraj da su svi segmenti isključeni pre nego što započneš
    activate_digit(digit_index)  # Aktiviraj odgovarajuću cifru
    for segment_index in range(8):  # Za svaki segment (A, B, C, D, E, F, G, DP)
        SEGMENTS[segment_index].value(1)  # Isključi trenutni segment (početno svi ugašeni)
        sleep(0.2)  # Drži segment uključen 0.2 sekunde
        SEGMENTS[segment_index].value(0)  # Uključi trenutni segment odmah
    sleep(0.5)  # Pauza pre nego što se pređe na sledeću cifru

# Funkcija za prikazivanje broja "8" na svim segmentima
def display_eight():
    deactivate_all_segments()  # Osiguraj da su svi segmenti isključeni pre nego što započneš
    # Prikaz broja "8" na 7-segmentnom displeju (svi segmenti uključeni)
    for i in range(4):  # Prvo D1, zatim D2, D3, D4
        activate_digit(i)  # Aktiviraj odgovarajuću cifru
        # Uključi sve segmente (A, B, C, D, E, F, G, DP) za broj "8"
        for segment_index in range(8):
            SEGMENTS[segment_index].value(0)  # Uključi svaki segment za anodni displej
        sleep(0.02)  # Prikaz "8" traje 1 sekundu
        deactivate_all_digits()  # Isključi sve cifre
        sleep(0.3)  # Pauza pre nego što pređe na sledeću cifru

# Funkcija za isključivanje svih segmenata
def deactivate_all_segments():
    for segment in SEGMENTS:
        segment.value(1)  # Isključi sve segmente (logički visoka vrednost za anodni displej)

# Glavna petlja
while True:
    deactivate_all_segments()  # Uverite se da su segmenti svi ugašeni na početku
    for digit_index in range(4):  # Prvo D1, zatim D2, D3, D4
        display_segments_on_digit(digit_index)  # Prikazivanje segmenata jedan po jedan na svakoj cifri
    display_eight()  # Kada završi sa prikazom segmenata, prikaži broj 8 na svim ciframa





