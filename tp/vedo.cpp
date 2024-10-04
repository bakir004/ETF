#include <iostream>

class Klasa {
    int x;
public:
    Klasa() {x=5;}
    void Funkcija(int c) const { std::cout << c << "\n"; }
    // void Funkcija() { std::cout << "Nekonstanta\n"; }
    Klasa &operator=(Klasa v) {
        std::cout << "Kopirajuci\n"; return *this;
    }
    Klasa &operator=(Klasa&& v){std::cout << "Rvrijednost\n";return *this;}
};

int F(int x) {
    return x;
}

int main() {
    Klasa k1;
    Klasa k2;
    return 0;
}
