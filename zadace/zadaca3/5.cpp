#include <iostream>
#include <stdexcept>
#include <vector>

struct Distrikt {
    int broj_distrikta;
    Distrikt *sljedeci;
};

std::vector<int> Razbrajanje(int N, int M) {
    Distrikt *pocetak= nullptr;
    try {
        pocetak = new Distrikt;
    } catch(...) {
        throw std::domain_error("Nema memorije!");
    }
    pocetak->broj_distrikta = 1;

    Distrikt *prosli = pocetak;

    int brojAlociranih = 1;
    try {
        for(int i = 2; i <= N; i++) {
            Distrikt *novi = new Distrikt;
            novi->broj_distrikta = i;
            prosli->sljedeci = novi;
            novi->sljedeci = nullptr;
            prosli = novi;
            brojAlociranih++;
        }
    } catch(...) {
        for(int i = 0; i < brojAlociranih; i++) {
            auto trenutni = pocetak;
            pocetak = pocetak->sljedeci;
            delete trenutni;
        }
        throw;
    }
    prosli->sljedeci = pocetak;
    std::vector<int> redoslijed;

    Distrikt *trenutni = pocetak;
    Distrikt *prijeTrenutnog = pocetak;
    for(int i = 0; i < N-1; i++) prijeTrenutnog = prijeTrenutnog->sljedeci;
    while(N > 0) {
        // akcija izbacivanja iz liste i dodavanja u vektor
        redoslijed.push_back(trenutni->broj_distrikta);
        auto zaObrisati = trenutni;
        trenutni = trenutni->sljedeci;
        prijeTrenutnog->sljedeci = trenutni;
        delete zaObrisati;
        N--;
        // end akcija

        for(int i = 0; i < M-1; i++) { // M-1 jer smo vec pomjerili pokazivace izbacivanjem elementa iz liste
            prijeTrenutnog = prijeTrenutnog->sljedeci;
            trenutni = trenutni->sljedeci;
        }
    }
    return redoslijed;
}

int OdabirKoraka(int N, int K) {
    int teoretskiMaksimalanM = N*N; // koja je zapravo granica?
    for(int M = 1; M <= teoretskiMaksimalanM; M++) {
        std::vector<int> redoslijed = Razbrajanje(N, M);
        if(redoslijed[N-1] == K)
            return M;
    }
    return 0;
}

int main() {
    int N = 10, K = 5;
    int M = OdabirKoraka(N, K);
    std::cout << "M: " << M << "\n";
    for(auto e : Razbrajanje(N, M))
        std::cout << e << " ";
    return 0;
}
