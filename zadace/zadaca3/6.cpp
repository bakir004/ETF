#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

struct Distrikt {
    int broj_distrikta;
    std::shared_ptr<Distrikt> sljedeci;
};

std::vector<int> Razbrajanje(int N, int M) {
    std::shared_ptr<Distrikt> pocetak = nullptr;
    try {
        pocetak = std::shared_ptr<Distrikt>(new Distrikt, [](Distrikt *p) {
                p->sljedeci = nullptr;
                delete p;
                });
    } catch (...) {
        throw;
    }
    pocetak->broj_distrikta = 1;

    std::shared_ptr<Distrikt> prosli = pocetak;

    try {
        for (int i = 2; i <= N; i++) {
            std::shared_ptr<Distrikt> novi(new Distrikt, [](Distrikt *p) {
                    p->sljedeci = nullptr;
                    delete p;
                    });
            novi->broj_distrikta = i;
            prosli->sljedeci = novi;
            novi->sljedeci = nullptr;
            prosli = novi;
        }
    } catch (...) {
        prosli = nullptr;
        pocetak = nullptr;
        throw std::domain_error("GRESKA");
    }
    prosli->sljedeci = pocetak;
    std::vector<int> redoslijed;

    std::shared_ptr<Distrikt> trenutni = pocetak;
    std::shared_ptr<Distrikt> prijeTrenutnog = pocetak;
    for (int i = 0; i < N - 1; i++)
        prijeTrenutnog = prijeTrenutnog->sljedeci;
    pocetak = nullptr;
    while (N > 0) {
        // akcija izbacivanja iz liste i dodavanja u vektor
        redoslijed.push_back(trenutni->broj_distrikta);
        trenutni = trenutni->sljedeci;
        prijeTrenutnog->sljedeci = trenutni;
        N--;
        // end akcija

        for (int i = 0; i < M - 1; i++) { // M-1 jer smo vec pomjerili pokazivace
                                          // izbacivanjem elementa iz liste
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
    int teoretskiMaksimalanM = N * N; // koja je zapravo granica?
    for (int M = 1; M <= teoretskiMaksimalanM; M++) {
        std::vector<int> redoslijed = Razbrajanje(N, M);
        if (redoslijed[N - 1] == K)
            return M;
    }
    return 0;
}

int main() {
    int N = 10, K = 3;
    std::cout << "Unesite broj distrikta u gradu: ";
    std::cin >> N;
    std::cout << "Unesite redni broj distrikta u kojem se nalazi restoran: ";
    std::cin >> K;
    int M = OdabirKoraka(N, K);
    std::cout << "Trazeni korak: " << M;
    return 0;
}
