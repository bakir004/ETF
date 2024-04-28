#include <iostream>

int Cifre(long long int n, int &c_min, int &c_max) {
    c_min = 10;
    c_max = 0;
    if(n < 0) n *= -1;
    int brojCifara = 0;
    while(n > 0) {
        int cifra = n%10;
        n /= 10;
        c_min = std::min(c_min, cifra);
        c_max = std::max(c_max, cifra);
        brojCifara++;
    }
    return brojCifara;
}

int main() {
    long long int n;
    int a, b;
    std::cin >> n;
    std::cout << Cifre(n, a, b) << "\n";
    std::cout << "MIN: " << a << ", MAX: " << b;
}

