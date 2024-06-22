#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iomanip>

class Berza {
private:
    std::vector<int> cijene;
    int minCijena;
    int maxCijena;
    bool isKreirana; 
public:
    Berza(int min, int max) : minCijena(min), maxCijena(max), isKreirana(true) {
        if (minCijena < 0 || maxCijena <= 0 || minCijena > maxCijena) {
            isKreirana = false; // Postavi isKreirana na false ako su granice neispravne
            throw std::range_error("Ilegalne granicne cijene");
        }
    }

    Berza(int max) : Berza(0, max) { }

    void RegistrirajCijenu(int cena) {
        if (cena < minCijena || cena > maxCijena) {
            throw std::range_error("Ilegalna cijena");
        }
        cijene.push_back(cena);
    }

    int DajBrojRegistriranihCijena() const {
        return cijene.size();
    }

    void BrisiSve() {
        cijene.clear();
    }

    int DajMinimalnuCijenu() const {
        if (cijene.empty()) {
            throw std::range_error("Nema registriranih cijena");
        }
        return *std::min_element(cijene.begin(), cijene.end());
    }

    int DajMaksimalnuCijenu() const {
        if (cijene.empty()) {
            throw std::range_error("Nema registriranih cijena");
        }
        return *std::max_element(cijene.begin(), cijene.end());
    }

    bool operator!() const {
        return cijene.empty();
    }

    int DajBrojCijenaVecihOd(int vrijednost) const {
        if (cijene.empty()) {
            throw std::range_error("Nema registriranih cijena");
        }
        return std::count_if(cijene.begin(), cijene.end(), std::bind(std::less<int>(), vrijednost, std::placeholders::_1));
    }

    void Ispisi() const {
        if (cijene.empty()) {
            throw std::range_error("Nema registriranih cijena");
        }
        std::vector<int> tempCijene = cijene;
        std::sort(tempCijene.begin(), tempCijene.end(), std::greater<int>());
        for (auto cena : tempCijene) {
            double km = static_cast<double>(cena) / 100.0;
            std::cout << std::fixed << std::setprecision(2) << km << std::endl;
        }
    }

    int operator[](int i) const {
        if (i < 1 || i > static_cast<int>(cijene.size())) {
            throw std::range_error("Neispravan indeks");
        }
        return cijene[i - 1];
    }

    Berza& operator++() {
        for (auto &cijena : cijene) {
            if (cijena + 100 > maxCijena) {
                throw std::range_error("Prekoracen dozvoljeni opseg cijena");
            }
            cijena += 100;
        }
        return *this;
    }

    Berza operator++(int) {
        Berza temp(*this);
        ++(*this);
        return temp;
    }

    Berza& operator--() {
        for (auto &cijena : cijene) {
            if (cijena - 100 < minCijena) {
                throw std::range_error("Prekoracen dozvoljeni opseg cijena");
            }
            cijena -= 100;
        }
        return *this;
    }

    Berza operator--(int) {
        Berza temp(*this);
        --(*this);
        return temp;
    }

    Berza operator-() const {
        Berza negirana(*this);
        for (auto &cijena : negirana.cijene) {
            cijena = maxCijena - cijena;
        }
        return negirana;
    }

    friend Berza operator+(const Berza& b, int value) {
        Berza nova(b);
        for (auto& cena : nova.cijene) {
            if (cena + value > nova.maxCijena) {
                throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
            }
            cena += value;
        }
        return nova;
    }

    friend Berza operator-(const Berza &b, int vrijednost) {
        return b + (-vrijednost);
    }

    friend Berza operator+(int vrijednost, const Berza &b) {
        return b + vrijednost;
    }

    friend Berza operator-(int vrijednost, const Berza &b) {
        return -b + vrijednost;
    }

    Berza &operator+=(int vrijednost) {
        *this = *this + vrijednost;
        return *this;
    }

    Berza &operator-=(int vrijednost) {
        *this = *this - vrijednost;
        return *this;
    }

    bool operator==(const Berza& other) const {
        return cijene == other.cijene;
    }

    bool operator!=(const Berza& other) const {
        return !(*this == other);
    }   

    bool Kreirana() const { 
        return isKreirana;
    }
};
int main() {
    try {
        Berza A(1000);
        std::cout << "Berza A uspjesno kreirana." << std::endl;
        Berza B(-500);
        std::cout << "Berza B uspjesno kreirana." << std::endl;
    } catch (const std::range_error& e) {
        std::cout << "Izuzetak: " << e.what() << std::endl;
    }
}
