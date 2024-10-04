#include <deque>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using std::begin;
using std::end;

template <typename T1, typename T2> auto Izmnozi(T1 n1, T2 n2) {
    return n1 * n2;
}

std::string Izmnozi(std::string n1, std::string n2) {
    throw std::logic_error("Nije ispunjena pretpostavka o komutativnosti");
}

template <typename Kontejner1, typename Kontejner2>
auto KreirajTablicuMnozenja(Kontejner1 &k1, Kontejner2 &k2) {
    int vel1 = 0, vel2 = 0;
    auto k1Begin = begin(k1);
    auto k2Begin = begin(k2);
    while (k1Begin++ != end(k1))
        vel1++;
    while (k2Begin++ != end(k2))
        vel2++;
    if (vel1 != vel2)
        throw std::range_error("Kontejneri nisu iste duzine");

    int n = (vel1 == vel2) * (vel1);
    if (n == 0)
        throw std::domain_error("Kontejneri su prazni");
    typedef std::remove_reference_t<decltype(*begin(k1))> T1;
    typedef std::remove_reference_t<decltype(*begin(k2))> T2;

    auto rez = Izmnozi(*begin(k1), *begin(k2));
    typedef decltype(rez) TRez;

    TRez **tablica{};
    try {
        tablica = new TRez *[n] {};
        tablica[0] = new TRez[n * (n + 1) / 2];
        for (int i = 1; i < n; i++)
            tablica[i] = tablica[i - 1] + i;
    } catch (...) {
        delete[] tablica;
        throw std::range_error("Nema dovoljno memorije");
    }

    auto pocetak1 = begin(k1);
    int counter1 = 0, counter2 = 0;
    while (pocetak1 != end(k1)) {
        auto pocetak2 = begin(k2);
        auto kraj2 = begin(k2) + 1;
        counter2 = 0;
        for (int i = 0; i < counter1; i++)
            kraj2++;
        while (pocetak2 != kraj2) {
            try {
                TRez proizvod1 = Izmnozi(*pocetak1, *pocetak2);
                TRez proizvod2 = Izmnozi(*pocetak2, *pocetak1);
                if (proizvod1 != proizvod2)
                    throw std::logic_error(
                            "Nije ispunjena pretpostavka o komutativnosti");
                tablica[counter1][counter2] = proizvod1;
                counter2++;
                pocetak2++;
            } catch (...) {
                delete[] tablica[0];
                delete[] tablica;
                throw std::logic_error("Nije ispunjena pretpostavka o komutativnosti");
            }
        }
        pocetak1++;
        counter1++;
    }
    return tablica;
}

int main() {
    int n;
    std::cout << "Duzina sekvenci: ";
    std::cin >> n;
    std::cout << "Elementi prve sekvence: ";
    std::vector<double> k1(n);
    for (int i = 0; i < n; i++)
        std::cin >> k1[i];
    std::cout << "Elementi druge sekvence: ";
    std::deque<double> k2(n);
    for (int i = 0; i < n; i++)
        std::cin >> k2[i];

    try {
        auto tablica = KreirajTablicuMnozenja(k1, k2);
        std::cout << "Tablica mnozenja:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i + 1; j++) {
                std::cout << tablica[i][j] << " ";
            }
            std::cout << std::endl;
        }
        delete[] tablica[0];
        delete[] tablica;
    } catch (const std::exception &err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}
