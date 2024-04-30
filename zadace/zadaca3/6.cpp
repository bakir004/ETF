#include <iostream>
#include <memory>
#include <vector>

struct Distrikt {
    int broj_distrikta;
    std::shared_ptr<Distrikt> sljedeci;
};

std::vector<int> Razbrajanje(int N, int M) {
    std::shared_ptr<Distrikt> prosli(new Distrikt);
    prosli->broj_distrikta = 1;

    std::shared_ptr<Distrikt> pocetak = prosli;

    for(int i = 2; i <= N; i++) {
        auto noviPtr = new Distrikt;
        std::shared_ptr<Distrikt> novi(new Distrikt);
        novi->broj_distrikta = i;
        prosli->sljedeci = novi;
        prosli = novi;
    }
    prosli->sljedeci = pocetak;
    std::vector<int> redoslijed;

    std::shared_ptr<Distrikt> trenutni = pocetak;
    std::shared_ptr<Distrikt> prijeTrenutnog = pocetak;
    for(int i = 0; i < N-1; i++) prijeTrenutnog = prijeTrenutnog->sljedeci;
    while(N > 0) {
        // akcija izbacivanja iz liste i dodavanja u vektor
        redoslijed.push_back(trenutni->broj_distrikta);
        trenutni = trenutni->sljedeci;
        prijeTrenutnog->sljedeci = trenutni;
        N--;
        // end akcija

        for(int i = 0; i < M-1; i++) { // M-1 jer smo vec pomjerili pokazivace izbacivanjem elementa iz liste
            prijeTrenutnog = prijeTrenutnog->sljedeci;
            trenutni = trenutni->sljedeci;
        }
    }
    trenutni->sljedeci = nullptr;
    trenutni = nullptr;
    prijeTrenutnog = nullptr;
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
    int N = 11, K = 2;
    int M = OdabirKoraka(N, K);
    std::cout << "M: " << M << "\n";
    for(auto e : Razbrajanje(N, M))
        std::cout << e << " ";
    return 0;
}

