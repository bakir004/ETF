#include <iostream>
#include <vector>
#include <cmath>
#include <deque>
#include <algorithm>

using std::begin;
using std::end;

int SumaCifara(int n) {
    int suma = 0;
    while(n > 0) {
        suma += n%10;
        n/=10;
    }
    return suma;
}

bool ProstaSumaCifara(int n) {
    int sumaCifara = SumaCifara(n);
    for(int i = 2; i <= sqrt(sumaCifara); i++)
        if(n % i == 0) return false;
    return true;
}

template <typename T, typename CompareFuncType>
T MojRemoveIf(T pocetak, T kraj, CompareFuncType f) {
    T pocetakKopija = pocetak;
    while(pocetak != kraj) {
        if(!f(*pocetak))
            *pocetakKopija++ = *pocetak;
        pocetak++;
    }  
    return pocetakKopija;
}

int main() {
    int n;
    std::cout << "Unesite broj elemenata: ";
    std::cin >> n;
    std::vector<int> v(n);
    std::deque<int> dek(n);
    std::cout << "Unesite elemente: ";
    // unos
    std::for_each(begin(v), end(v), [](auto &c){ std::cin >> c; });
    // sortiranje
    std::sort(begin(v), end(v), [](const int &e1, const int &e2){ return SumaCifara(e1) < SumaCifara(e2); });
    
    // prebrojavanje
    int brojOnihSaProstimBrojemCifara = std::count_if(begin(v), end(v), ProstaSumaCifara);
    // kopiranje
    auto izaZadnjeg = std::copy_if(begin(v), end(v), begin(dek), ProstaSumaCifara );
    int velicinaDeka = izaZadnjeg - begin(dek);
    dek.resize(velicinaDeka); 
    // ispis kopiranih elemenata
    std::cout << "Kopirani elementi (" << brojOnihSaProstimBrojemCifara << "): \n";
    std::for_each(begin(dek), end(dek), [](auto &c){ std::cout << c << " "; });

    // izbacivanje iz originalnog vektora
    auto vektorIzaZadnjeg = MojRemoveIf(begin(v), end(v), ProstaSumaCifara );
    int velicinaVektora = vektorIzaZadnjeg - begin(v);
    v.resize(velicinaVektora);
    // ispis nakon izbacivanja
    std::cout << "\nPreostali elementi (" << velicinaVektora << "): \n";
    std::for_each(begin(v), end(v), [](auto &c){ std::cout << c << " "; });

    return 0;
}
