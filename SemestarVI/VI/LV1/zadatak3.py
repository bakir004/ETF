import numpy as np
import math

print("Zadatak 3a")
a = np.zeros((4, 3))
print(a)

print("\nZadatak 3b")
b = np.ones((2, 2))
print(b)

print("\nZadatak 3c")
c = np.arange(1, 11).reshape(10, 1)
print(c)

print("\nZadatak 3d")
M = np.array([
    [1,        2,  3],
    [1/3.6,    5,  23],
    [2**10.5, 42,  math.cos(80.841)]
])
print(M)

print("\nZadatak 3e")

MT = M.T
print("(a) Transponovana matrica:")
print(MT)

print("\n(b) MT + M:")
print(MT + M)

suma_prve_kolone = np.sum(M[:, 0])
print(f"\n(c) Suma prve kolone: {suma_prve_kolone}")
print("M * suma_prve_kolone:")
print(M * suma_prve_kolone)

print("\nZadatak 3f")

print("(a) Treći red:")
print(M[2, :])

print("\n(b) Druga kolona:")
print(M[:, 1])

print(f"\n(c) Element na lokaciji (0, 2): {M[0, 2]}")

print("\nZadatak 3g")

def kvadratna(niz):
    n = len(niz)
    strana = int(math.sqrt(n))
    if strana * strana == n:
        return niz.reshape(strana, strana)

v1 = np.arange(1, 10)
print("Ulazni niz:", v1)
print("Kvadratna matrica:")
print(kvadratna(v1))

v2 = np.arange(1, 8)
print("\nUlazni niz:", v2)
rezultat = kvadratna(v2)
if rezultat is None:
    print("Nije moguće pretvoriti u kvadratnu matricu.")
