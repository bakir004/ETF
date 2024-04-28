#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <new>
#include <stdexcept>
#include <iterator>
#include <vector>

int BrojCifara(int n) {
    if(n == 0) return 1;
    int brojac = 0;
    while(n>0) {
        n /= 10;
        brojac++;
    }
    return brojac;
}

template <typename IterTip>
auto DigitMatrix(IterTip pocetak, IterTip kraj) {
    int brojRedova = std::distance(pocetak, kraj);
    if(brojRedova == 0) throw std::domain_error("Blok je prazan!");
    int **matrica{};
    matrica = new int*[brojRedova];
    int index = 0; 
    try {
        while(pocetak != kraj) {
            int broj = *pocetak;
            if(broj < 0)
                throw std::range_error("Negativan broj!");
            int brojCifara = BrojCifara(*pocetak);
            matrica[index] = new int[brojCifara];
            std::vector<int> cifre;
            if(broj == 0) cifre.push_back(0);
            for(int i = 0; broj > 0; i++) {
                int cifra = broj % 10;
                broj /= 10;
                cifre.push_back(cifra);
            }
            std::reverse(cifre.begin(), cifre.end());
            for(int i = 0; i < brojCifara; i++)
                matrica[index][i] = cifre[i];
            index++;
            pocetak++;
        }
    } catch(const std::exception& err) {
        // ovdje index glumi broj uspjesno alociranih redova
        for(int i = 0; i < index; i++)
            delete[] matrica[i];
        delete[] matrica;
        throw;
    }
    return matrica;
}

int main() {
    int n;
    std::cout << "Unesite broj elemenata: ";
    std::cin >> n;
    std::deque<int> dek(n);
    std::cout << "Unesite elemente: ";
    for(int i = 0; i < n; i++)
        std::cin >> dek[i];

    try {
        auto matrica = DigitMatrix(dek.begin(), dek.end());
        std::cout << "\nRezultantna matrica cifara:\n";
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < BrojCifara(dek[i]); j++)
                std::cout << matrica[i][j] << " ";
            std::cout << std::endl;
        }
    } catch(const std::exception& err) {
        std::cout << err.what() << "\n";
    }
    
    return 0;
}
