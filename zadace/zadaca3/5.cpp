#include <iostream>
#include <vector>

struct Distrikt {
    int broj_distrikta;
    Distrikt *sljedeci;
};

std::vector<int> Razbrajanje(int N, int M) {
    Distrikt *prosli = new Distrikt;
    prosli->broj_distrikta = 1;

    Distrikt *pocetak = prosli;

    for(int i = 2; i <= N; i++) {
        Distrikt *novi = new Distrikt;
        novi->broj_distrikta = i;
        prosli->sljedeci = novi;
        prosli = novi;
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
    for(int i = 1; i <= N; i++) {
        std::vector<int> redoslijed = Razbrajanje(N, i);
        if(redoslijed[N-1] == K)
            return i;
    }
    return 0;
}

int main() {
    int N = 10, K = 2;
    int M = OdabirKoraka(N, K);
    std::cout << "M: " << M << "\n";
    for(auto e : Razbrajanje(N, M))
        std::cout << e << " ";
    return 0;
}
