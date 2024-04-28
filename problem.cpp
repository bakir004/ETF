#include <iostream>

template <typename T>
void Funkcija(T n, decltype(n) f(decltype(n))) {
    T novi = f(n);
    std::cout << novi << "\n";
}

int main() {
    
    // Funkcija<int>(5, [](int x){ return x/2 + 20; });
    // Funkcija<char>('a', [](char x){ return char(x/2 + 20); });
    // Funkcija<double>(5., [](double x){ return x/2 + 20; });
    
    // zelimo ovo
    // Funkcija(5, [](int x){ return x/2 + 20; });
    // Funkcija(5., [](double x){ return x/2 + 20; });
    //
    // // i sprijeciti ovo
    Funkcija(5, [](double x){ return x/2 + 20; });
    //
    // auto f = [](int x){ return x/2 + 20; };
    // Funkcija(5, f);
    return 0;
}
