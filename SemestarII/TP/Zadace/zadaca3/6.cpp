#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

struct Distrikt {
    int broj_distrikta;
    std::shared_ptr<Distrikt> sljedeci;
};

std::vector<int> Razbrajanje(int N, int M) {
    if (N == 0 || M == 0)
        throw std::domain_error(
                "Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni "
                "broj bloka ne moze biti veci od broja blokova");
    std::shared_ptr<Distrikt> pocetak = std::make_shared<Distrikt>();
    std::shared_ptr<Distrikt> prosli = pocetak;
    pocetak->broj_distrikta = 1;

    for (int i = 2; i <= N; i++) {
        std::shared_ptr<Distrikt> novi = std::make_shared<Distrikt>();
        novi->broj_distrikta = i;
        prosli->sljedeci = novi;
        novi->sljedeci = nullptr;
        prosli = novi;
    }
    prosli->sljedeci = pocetak;
    std::vector<int> redoslijed;

    std::shared_ptr<Distrikt> trenutni = pocetak;
    std::shared_ptr<Distrikt> prijeTrenutnog = pocetak;
    for (int i = 0; i < N - 1; i++)
        prijeTrenutnog = prijeTrenutnog->sljedeci;
    while (N > 0) {
        redoslijed.push_back(trenutni->broj_distrikta);
        trenutni = trenutni->sljedeci;
        prijeTrenutnog->sljedeci = trenutni;
        N--;

        for (int i = 0; i < M - 1; i++) { // M-1 jer smo vec pomjerili pokazivace
            prijeTrenutnog = prijeTrenutnog->sljedeci;
            trenutni = trenutni->sljedeci;
        }
    }
    pocetak = nullptr;
    trenutni->sljedeci = nullptr;
    trenutni = nullptr;
    prijeTrenutnog = nullptr;
    return redoslijed;
}

int OdabirKoraka(int N, int K) {
    if (N == 0 || K == 0 || K > N)
        throw std::domain_error(
                "Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni "
                "broj bloka ne moze biti veci od broja blokova");
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
    try {
        int M = OdabirKoraka(N, K);
        std::cout << "Trazeni korak: " << M;
    } catch (std::exception &err) {
        std::cout << err.what();
    }
    return 0;
}
