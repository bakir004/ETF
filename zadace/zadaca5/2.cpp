#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

class Berza {
    std::vector<int> cijene;
    int min, max;
    bool DaLiImaPrekoracenja();
public:
    Berza(int min, int max);
    explicit Berza(int max);
    void RegistrirajCijenu(int cijena);
    int DajBrojRegistriranihCijena() const noexcept;
    void BrisiSve() noexcept;
    int DajMinimalnuCijenu() const;
    int DajMaksimalnuCijenu() const;
    bool operator!() const;
    int DajBrojCijenaVecihOd(int x) const;
    void Ispisi() const;
    int operator[](int index) const;
    Berza& operator++();
    Berza operator++(int);
    Berza& operator--();
    Berza operator--(int);
    friend Berza operator-(const Berza& b);

    friend Berza operator+(const Berza& b1, int);
    friend Berza operator-(const Berza& b1, int);
    friend Berza operator+(int, const Berza& b1);
    friend Berza operator-(int, const Berza& b1);

    friend Berza operator-(const Berza& b1, const Berza& b2);
    friend Berza operator+(const Berza& b1, const Berza& b2);
    Berza& operator+=(int x);
    Berza& operator+=(Berza b);
    Berza& operator-=(const Berza& b);
    Berza& operator-=(int x);

    friend bool operator==(const Berza& b1, const Berza& b2);
    friend bool operator!=(const Berza& b1, const Berza& b2);
};
Berza::Berza(int max) {
    if(max < 0) throw std::range_error("Ilegalne granicne cijene");
    this->max = max;
    this->min = 0;
}
Berza::Berza(int min, int max)  {
    if(min < 0 || max < 0 || max < min) throw std::range_error("Ilegalne granicne cijene");
    this->min = min;
    this->max = max;
}
void Berza::RegistrirajCijenu(int cijena) {
    if(cijena < min || cijena > max) throw std::range_error("Ilegalna cijena");
    cijene.push_back(cijena);
}

bool operator==(const Berza& b1, const Berza& b2) {
    return b1.min == b2.min && b1.max == b2.max && b1.cijene.size() == b2.cijene.size() && std::equal(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin());
}
bool operator!=(const Berza& b1, const Berza& b2) {
    return b1.min != b2.min || b1.max != b2.max || b1.cijene.size() != b2.cijene.size() || !std::equal(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin());
}
Berza& Berza::operator+=(Berza b) {
    Berza nova = *this + b;
    if(nova.DaLiImaPrekoracenja()) throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    cijene = nova.cijene;
    return *this;
}
Berza& Berza::operator+=(int x) {
    Berza nova = *this + x;
    if(nova.DaLiImaPrekoracenja()) throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    cijene = nova.cijene;
    return *this;
}
Berza& Berza::operator-=(const Berza& b) {
    Berza nova = *this - b;
    if(nova.DaLiImaPrekoracenja()) throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    cijene = nova.cijene;
    return *this;
}
Berza& Berza::operator-=(int x) {
    Berza nova = *this - x;
    if(nova.DaLiImaPrekoracenja()) throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    cijene = nova.cijene;
    return *this;
}
Berza operator+(const Berza& b1, const Berza& b2) {
    if(b1.cijene.size() != b2.cijene.size()) throw std::domain_error("Nesaglasni operandi");
    Berza nova(b1.min, b1.max);
    nova.cijene = std::vector<int>(b1.cijene.size());
    std::transform(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin(), nova.cijene.begin(), std::plus<int>());
    nova.DaLiImaPrekoracenja();
    return nova;
}
Berza operator-(const Berza& b1, const Berza& b2) {
    if(b1.cijene.size() != b2.cijene.size()) throw std::domain_error("Nesaglasni operandi");
    Berza nova(b1.min, b1.max);
    nova.cijene = std::vector<int>(b1.cijene.size());
    std::transform(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin(), nova.cijene.begin(), std::minus<int>());
    if(nova.DaLiImaPrekoracenja()) throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    return nova;
}
Berza operator+(const Berza& b1, int x) {
    Berza nova(b1.min, b1.max);
    nova.cijene = std::vector<int>(b1.cijene.size());
    std::transform(b1.cijene.begin(),b1.cijene.end(),nova.cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, x));
    if(nova.DaLiImaPrekoracenja()) {
        std::transform(b1.cijene.begin(),b1.cijene.end(),nova.cijene.begin(),std::bind(std::minus<int>(), std::placeholders::_1, x));
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    return nova;
}
Berza operator+(int x, const Berza& b1) { return b1 + x; }
Berza operator-(const Berza& b1, int x) {
    Berza nova(b1.min, b1.max);
    nova.cijene = std::vector<int>(b1.cijene.size());
    std::transform(b1.cijene.begin(),b1.cijene.end(),nova.cijene.begin(),std::bind(std::minus<int>(), std::placeholders::_1, x));
    if(nova.DaLiImaPrekoracenja()) {
        std::transform(b1.cijene.begin(),b1.cijene.end(),nova.cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, x));
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    return nova;
}
Berza operator-(int x, const Berza& b1) {
    Berza nova(b1.min, b1.max);
    nova.cijene = std::vector<int>(b1.cijene.size());
    std::transform(b1.cijene.begin(),b1.cijene.end(),nova.cijene.begin(),std::bind(std::minus<int>(), x, std::placeholders::_1));
    if(nova.DaLiImaPrekoracenja()) {
        std::transform(b1.cijene.begin(),b1.cijene.end(),nova.cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, x));
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    return nova;
}
Berza operator-(const Berza& b) {
    Berza nova(b.min, b.max);
    nova.cijene = std::vector<int>(b.cijene.size());
    int zbir = b.min + b.max;
    std::transform(b.cijene.begin(),b.cijene.end(),nova.cijene.begin(),std::bind(std::minus<int>(), zbir, std::placeholders::_1));
    return nova;
}
bool Berza::DaLiImaPrekoracenja() {
    int prekoracenja = std::count_if(cijene.begin(),cijene.end(),std::bind(std::greater<int>(), std::placeholders::_1, max));
    prekoracenja += std::count_if(cijene.begin(),cijene.end(),std::bind(std::less<int>(), std::placeholders::_1, min));
    return prekoracenja != 0;
}
Berza& Berza::operator++() {
    std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, 100));
    if(DaLiImaPrekoracenja()) {
        std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::minus<int>(), std::placeholders::_1, 100));
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    return *this;
}
Berza Berza::operator++(int) {
    Berza stara = *this;
    std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, 100));
    if(DaLiImaPrekoracenja()) {
        std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::minus<int>(), std::placeholders::_1, 100));
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    return stara;
}
Berza& Berza::operator--() {
    std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::minus<int>(), std::placeholders::_1, 100));
    if(DaLiImaPrekoracenja()) {
        std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, 100));
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    return *this;
}
Berza Berza::operator--(int) {
    Berza stara = *this;
    std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::minus<int>(), std::placeholders::_1, 100));
    if(DaLiImaPrekoracenja()) {
        std::transform(cijene.begin(),cijene.end(),cijene.begin(),std::bind(std::plus<int>(), std::placeholders::_1, 100));
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    return stara;
}
int Berza::operator[](int index) const {
    if(index < 1 || index > cijene.size()) throw std::range_error("Neispravan indeks");
    if(cijene.size() == 0) throw std::range_error("Nema registriranih cijena");
    return cijene[index-1];
}
void Berza::Ispisi() const {
    std::cout << std::fixed << std::setprecision(2);
    std::vector<double> sortiraneCijene(cijene.size());
    std::copy(cijene.begin(), cijene.end(), sortiraneCijene.begin());
    std::sort(sortiraneCijene.begin(), sortiraneCijene.end(), std::greater<int>());
    std::transform(sortiraneCijene.begin(), sortiraneCijene.end(), sortiraneCijene.begin(),
            std::bind(std::divides<double>(), std::placeholders::_1, 100));
    std::copy(sortiraneCijene.begin(), sortiraneCijene.end(), std::ostream_iterator<double>(std::cout, "\n"));
}
int Berza::DajBrojCijenaVecihOd(int x) const {
    if(!*this) throw std::range_error("Nema registriranih cijena");
    auto kriterij = std::bind(std::greater<int>(), std::placeholders::_1, x);
    return std::count_if(cijene.begin(), cijene.end(), kriterij);
}
bool Berza::operator!() const { return cijene.size() == 0; }
int Berza::DajMaksimalnuCijenu() const {
    if(cijene.size() == 0) throw std::range_error("Nema registriranih cijena");
    return *std::max_element(cijene.begin(), cijene.end(), [](int a, int b){ return a < b; });
}
int Berza::DajMinimalnuCijenu() const {
    if(cijene.size() == 0) throw std::range_error("Nema registriranih cijena");
    return *std::min_element(cijene.begin(), cijene.end(), [](int a, int b){ return a < b; });
}
void Berza::BrisiSve() noexcept { cijene.clear(); }
int Berza::DajBrojRegistriranihCijena() const noexcept { return cijene.size(); }

int main() {
    Berza b1(100, 500);
    Berza b2(100, 500);
    Berza b3(200, 600);

    b1.RegistrirajCijenu(300);
    b1.RegistrirajCijenu(400);
    b2.RegistrirajCijenu(300);
    b2.RegistrirajCijenu(400);
    b3.RegistrirajCijenu(300);
    b3.RegistrirajCijenu(500);

    if (b1 == b2) {
        std::cout << "b1 je jednak b2." << std::endl;
    } else {
        std::cout << "b1 nije jednak b2." << std::endl;
    }

    if (b1 == b3) {
        std::cout << "b1 je jednak b3." << std::endl;
    } else {
        std::cout << "b1 nije jednak b3." << std::endl;
    }

    if (b1 != b3) {
        std::cout << "b1 nije jednak b3." << std::endl;
    } else {
        std::cout << "b1 je jednak b3." << std::endl;
    }

    if (b1 != b2) {
        std::cout << "b1 nije jednak b2." << std::endl;
    } else {
        std::cout << "b1 je jednak b2." << std::endl;
    }

    try {
        std::cout << "Cijena b1 na indeksu 1: " << b1[1] << std::endl;
        std::cout << "Cijena b2 na indeksu 2: " << b2[2] << std::endl;
        std::cout << "Cijena b3 na indeksu 3: " << b3[3] << std::endl;
    } catch (const std::range_error& e) {
        std::cout << "Izuzetak: " << e.what() << std::endl;
    }
    return 0;
}

