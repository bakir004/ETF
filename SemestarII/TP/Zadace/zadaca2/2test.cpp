#include <iostream>
#include <deque>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <vector>
#define PRECIZNOST 3

using std::begin;
using std::end;

template <typename Matrica1, typename Matrica2, typename F>
auto GeneraliziraniKroneckerovProizvod(Matrica1 &m1, Matrica2 &m2, F f) {
    bool grbavPrvi = false, grbavDrugi = false;
    int visinaM1, sirinaM1, visinaM2, sirinaM2;
    visinaM1 = std::distance(begin(m1),end(m1));
    if(visinaM1 == 0)
        grbavPrvi = true;
    else
        sirinaM1 = std::distance(begin(*(begin(m1))), end(*(begin(m1))));
    visinaM2 = std::distance(begin(m2), end(m2));
    if(visinaM2 == 0)
        grbavDrugi = true;
    else
        sirinaM2 = std::distance(begin(*(begin(m2))), end(*(begin(m2))));
    int rezultantnaVisina = visinaM1 * visinaM2;
    int rezultantnaSirina = sirinaM1 * sirinaM2;

    // provjera grbavosti
    auto pocetak1 = begin(m1);
    for(int i = 0; i < std::distance(begin(m1), end(m1)); i++, pocetak1++) {
        int trenutnaSirina = std::distance(begin(*pocetak1), end(*pocetak1));
        if(trenutnaSirina != sirinaM1) { grbavPrvi = true; break; }
    }
    auto pocetak2 = begin(m2);
    for(int i = 0; i < std::distance(begin(m2), end(m2)); i++, pocetak2++) {
        int trenutnaSirina = std::distance(begin(*pocetak2), end(*pocetak2));
        if(trenutnaSirina != sirinaM2) { grbavDrugi = true; break; }
    }
    if(grbavPrvi && grbavDrugi)
        throw std::domain_error("Parametri nemaju formu matrice");
    else if(grbavPrvi)
        throw std::domain_error("Prvi parametar nema formu matrice");
    else if(grbavDrugi)
        throw std::domain_error("Drugi parametar nema formu matrice");
    // end provjera grbavosti

    // dakle, matrice nisu prazne niti grbave
    auto rez = f(*begin(*begin(m1)), *begin(*begin(m2)));
    typedef decltype(rez) T;
    typedef std::vector<std::vector<T>> MatricaT;
    typedef std::remove_reference_t<decltype(*begin(*begin(m1)))> T1;
    typedef std::remove_reference_t<decltype(*begin(*begin(m2)))> T2;
    MatricaT rezultantnaMatrica(rezultantnaVisina, std::vector<T>(rezultantnaSirina, 0));
    auto pocetakM1 = begin(m1);
    for(int i = 0; i < visinaM1; i++, pocetakM1++) {
        auto pocetakRedaM1 = begin(*pocetakM1);
        for(int j = 0; j < sirinaM1; j++, pocetakRedaM1++) {
            T1 element1 = *pocetakRedaM1;
            auto pocetakM2 = begin(m2);
            for(int k = 0; k < visinaM2; k++, pocetakM2++) {
                auto pocetakRedaM2 = begin(*pocetakM2);
                for(int l = 0; l < sirinaM2; l++, pocetakRedaM2++) {
                    T2 element2 = *pocetakRedaM2;
                    try {
                        rezultantnaMatrica.at(i * visinaM2 + k).at(j * sirinaM2 + l) = f(element1, element2);
                    } catch(...) {
                        throw std::runtime_error("Neocekivani problemi pri racunanju");
                    }
                }
            }
        }
    }
    return rezultantnaMatrica;
}

int Sirina(double x) {
    if(x == 0) return 1;
    int sirina = 0;
    if(x < 0) {
        sirina++;
        x = -x;
    }
    if(int(x) != x) sirina++;
    if(int(x) == 0) sirina++;

    int cijeliDioX = int(x);
    x -= cijeliDioX; 
    while(cijeliDioX > 0) {
        cijeliDioX/=10;
        sirina++;
    }
    int counterNula = 0;
    while(true) {
        x *= 10;
        if(int(x) == 0)
            counterNula++;
        else  {
            sirina+=counterNula+1;
            counterNula = 0;
        }
        if(counterNula >= PRECIZNOST)
            break;
        x -= int(x);
    }

    return sirina + 1;
}

#include <list>

template <typename TipElemenata>
class Pristupnik
{
    std::list<TipElemenata> &red;

public:
    Pristupnik(std::list<TipElemenata> &red) : red(red) {}
    TipElemenata &operator[](int j) { return *std::next(red.begin(), j); }
    TipElemenata operator[](int j) const { return *std::next(red.begin(), j); }
    auto begin() const { return red.begin(); }
    auto end() const { return red.end(); }
};

template <typename TipElemenata>
class SugavaMatrica
{
    std::list<std::list<TipElemenata>> mat;

public:
    SugavaMatrica(int m = 0, int n = 0) : mat(m, std::list<TipElemenata>(n)) {}
    Pristupnik<TipElemenata> operator[](int i) { return Pristupnik<TipElemenata>(*std::next(mat.begin(), i)); }
    const Pristupnik<TipElemenata> operator[](int i) const { return Pristupnik<TipElemenata>(*std::next(mat.begin(), i)); }
    auto begin() const { return mat.begin(); }
    auto end() const { return mat.end(); }
};

int main() {
    SugavaMatrica<int> m1(3, 3);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m1[i][j] = (i + 1) * (j + 1);

    SugavaMatrica<int> m2(3, 3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m2[i][j] = (i + 1) + (j + 1);

    auto result = GeneraliziraniKroneckerovProizvod(m1, m2, [](int x, int y)
                                                    { return x * y; });
    for (const auto &row : result)
    {
        for (const auto &elem : row)
            std::cout << elem << ' ';
        std::cout << '\n';
    }
    return 0;
}

