#include <exception>
#include <iostream>
#include <list>
#include <vector>

std::vector<int> Razbrajanje(int N, int M) {
    if (N == 0 || M == 0)
        throw std::domain_error(
                "Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni "
                "broj bloka ne moze biti veci od broja blokova");
    std::vector<int> redoslijed;
    std::list<int> lista;
    for (int i = 1; i <= N; i++)
        lista.push_back(i);
    auto it = lista.begin();
    while (!lista.empty()) {
        redoslijed.push_back(*it);
        it = lista.erase(it);
        if (it == lista.end())
            it = lista.begin();
        for (int i = 0; i < M - 1; i++) {
            it++;
            if (it == lista.end())
                it = lista.begin();
        }
    }
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
