#include <iostream>

template <typename T>
T Funkcija(T x, T f(T), int n) {
    T rezultat = f(x);
    for(int i = 1; i < n; i++) 
        rezultat = f(rezultat);
    return rezultat;
}

int main() {
    // preveliko je za n = 5
    std::cout << Funkcija<long long int>(10, [](long long int x) -> long long int { return 3 * x * x - 5; }, 4);
    return 0;
}