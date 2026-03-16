#include<iostream>

void AnalizaBroja(int x, int& brojCifara, int& brojParnih, int& raspon) {
    brojCifara = 0;
    brojParnih = 0;
    int najmanja = x % 10;
    int najveca = x % 10;

    while(x > 0) {
        int cifra = x % 10;
        // radimo sa cifrom sta hocemo
        brojCifara++;
        if(cifra % 2 == 0)
            brojParnih++;

        if(cifra < najmanja)
            najmanja = cifra;
        if(cifra > najveca)
            najveca = cifra;

        x = x / 10;
    }
    raspon = najveca - najmanja;
}

int main() {
    int A,B,C;
    AnalizaBroja(123456789, A, B, C);
    std::cout << A << " " << B << " " << C;

    return 0;
}
