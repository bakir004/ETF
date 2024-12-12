#include<iostream>
#include<string>

const float FAKTOR_PROSIRENJA = 1.5;
const int POCETNA_VELICINA = 10;
using namespace std;

template <typename K, typename V>
class Mapa {
public:
    virtual ~Mapa() {}
    virtual void obrisi() = 0;
    virtual void obrisi(const K& kljuc) = 0;
    virtual int brojElemenata() const = 0;
    virtual V& operator[] (const K& kljuc) = 0;
    virtual V operator[] (const K& kljuc) const = 0;
};

template <typename K, typename V>
class NizMapa : public Mapa<K, V> {
    std::pair<K, V>** niz;
    int broj_el;
    int kapacitet;
public:
    NizMapa() : niz(new std::pair<K, V>*[POCETNA_VELICINA]{}), broj_el(0), kapacitet(POCETNA_VELICINA) {}
    ~NizMapa() {
        for (int i = 0; i < broj_el; i++)
            delete niz[i];
        delete[] niz;
    }
    NizMapa(const NizMapa& nm) : niz(new std::pair<K, V>*[nm.kapacitet]{}), broj_el(nm.broj_el), kapacitet(nm.kapacitet) {
        for (int i = 0; i < nm.kapacitet; i++)
            if (nm.niz[i] != nullptr)
                niz[i] = new std::pair<K, V>(*nm.niz[i]);
    }
    NizMapa& operator=(const NizMapa& nm) {
        if (this == &nm)
            return *this;
        for (int i = 0; i < kapacitet; i++)
            delete niz[i];
        delete[] niz;

        broj_el = nm.broj_el;
        kapacitet = nm.kapacitet;
        niz = new std::pair<K, V>*[kapacitet]{};
        for (int i = 0; i < kapacitet; i++)
            if (nm.niz[i] != nullptr)
                niz[i] = new std::pair<K, V>(*nm.niz[i]);
        return *this;
    }
    V operator[](const K& kljuc) const {
        for (int i = 0; i < kapacitet; i++) {
            if (niz[i] != nullptr && niz[i]->first == kljuc)
                return niz[i]->second;
        }
        return V();
    }
    V& operator[](const K& kljuc) {
        for (int i = 0; i < kapacitet; i++) {
            if (niz[i] != nullptr && niz[i]->first == kljuc)
                return niz[i]->second;
        }

        for (int i = 0; i < kapacitet; i++) {
            if (niz[i] == nullptr) {
                niz[i] = new std::pair<K, V>(kljuc, V());
                broj_el++;
                return niz[i]->second;
            }
        }

        int novi_kapacitet = kapacitet * FAKTOR_PROSIRENJA;
        std::pair<K, V>** temp = new std::pair<K, V>*[novi_kapacitet]{};
        for (int i = 0; i < kapacitet; i++)
            temp[i] = niz[i];

        delete[] niz;
        niz = temp;
        kapacitet = novi_kapacitet;

        niz[broj_el] = new std::pair<K, V>(kljuc, V());
        broj_el++;
        return niz[broj_el - 1]->second;
    }
    void obrisi() {
        for (int i = 0; i < kapacitet; i++) {
            delete niz[i];
            niz[i] = nullptr;
        }
        broj_el = 0;
    }
    void obrisi(const K& kljuc) {
        for (int i = 0; i < kapacitet; i++) {
            if (niz[i] != nullptr && niz[i]->first == kljuc) {
                delete niz[i];
                niz[i] = nullptr;
                broj_el--;
                return;
            }
        }
    }
    int brojElemenata() const { return broj_el; }
    void ispisi() const {
        for (int i = 0; i < kapacitet; i++) {
            if (niz[i] != nullptr)
                std::cout << "(" << niz[i]->first << ", " << niz[i]->second << ")" << std::endl;
        }
    }
};

int main() {
    NizMapa<int, int> m1;
    m1[1] = 2;
    std::cout << m1[1] << std::endl;

    NizMapa<std::string, std::string> glavni_gradovi;
    glavni_gradovi["Bosna i Hercegovina"] = "Sarajevo";
    glavni_gradovi["Hrvatska"] = "Zagreb";
    glavni_gradovi["Sjedinjene Americke Drzave"] = "Washington";
    std::cout << glavni_gradovi["Bosna i Hercegovina"] << std::endl;
    std::cout << glavni_gradovi["Hrvatska"] << std::endl;

    NizMapa<int, std::string> m2;
    m2[1] = "jedan";
    m2[2] = "dva";
    m2[3] = "tri";
    std::cout << m2[1] << std::endl;
    std::cout << m2[2] << std::endl;
    std::cout << m2[3] << std::endl;
    m2.obrisi(2);
    std::cout << "Obrisano 2" << std::endl;
    m2.ispisi();
    std::cout << m2[2] << std::endl;
    std::cout << m2[3] << std::endl;

    return 0;
}
