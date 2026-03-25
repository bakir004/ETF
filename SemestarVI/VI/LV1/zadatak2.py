import math

print("Zadatak 2a")
lista = []
for i in range(10):
    el = float(input(f"Unesi element {i+1}: "))
    lista.append(el)

sortirana = sorted(lista)
print(f"Drugi najveci element: {sortirana[-2]}")

print("\nZadatak 2b")
n = int(input("Unesi n za Fibonacci: "))
if n <= 0:
    print("n mora biti pozitivan!")
elif n == 1:
    print(f"Fibonacci({n}) = 0")
elif n == 2:
    print(f"Fibonacci({n}) = 1")
else:
    a, b = 0, 1
    for _ in range(2, n):
        a, b = b, a + b
    print(f"Fibonacci({n}) = {b}")


print("\nZadatak 2c")
a = float(input("Unesi a: "))
b = float(input("Unesi b: "))
c = float(input("Unesi c: "))

diskriminanta = b ** 2 - 4 * a * c

if diskriminanta < 0:
    print("Rjesenja su kompleksna!")
elif diskriminanta == 0:
    print("Rjesenja su ista!")
    x = -b / (2 * a)
    print(f"x = {x}")
else:
    x1 = (-b + math.sqrt(diskriminanta)) / (2 * a)
    x2 = (-b - math.sqrt(diskriminanta)) / (2 * a)
    print(f"x1 = {x1}")
    print(f"x2 = {x2}")


print("\nZadatak 2d")

def fact(n):
    result = 1
    for i in range(2, n + 1):
        result *= i
    return result

def choose(n, k):
    return fact(n) // (fact(k) * fact(n - k))

print(f"fact(5) = {fact(5)}")
print(f"fact(10) = {fact(10)}")
print(f"choose(5, 2) = {choose(5, 2)}")
print(f"choose(10, 3) = {choose(10, 3)}")
