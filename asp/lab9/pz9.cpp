#include <iostream>
#include <stdexcept>
#include <string>
#include <ctime>
#include <functional>
#include <cstdlib>
#include <vector>

const float FAKTOR_PROSIRENJA = 1.5;
const int POCETNA_VELICINA = 10;
const int POCETNA_VELICINA_HASH_TABELE = 10;
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


template <typename K, typename V>
class HashMapa : public Mapa<K, V> {
    typedef std::pair<K, V> Par;
    int velicina;
    int kapacitet;
    Par** niz;
    unsigned int hash(std::string ulaz, int max) {
        unsigned int hashValue = 0;
        unsigned int prime = 37;
        for (char c : ulaz)
            hashValue = (hashValue * prime + (unsigned int)(c)) % max;
        return hashValue;
    }
    Par* DEL;
public:
    HashMapa(): velicina(0), DEL(new Par({K(),V()})), kapacitet(POCETNA_VELICINA_HASH_TABELE), niz(new Par*[kapacitet]{}) {
    }
    ~HashMapa() {
    }
    HashMapa(const HashMapa& nm) : niz(new Par*[nm.kapacitet]{}), velicina(nm.velicina), kapacitet(nm.kapacitet) {
    }
    HashMapa& operator=(const HashMapa& nm) {
    }
    V operator[](const K& kljuc) const {
        return V();
    }
    V& operator[](const K& kljuc) {
        unsigned int hashirano = this->hash(kljuc, kapacitet);
        if(niz[hashirano] == nullptr) {
            std::cout << "Kreiran " << kljuc << " na adresi " << hashirano << "\n";
            Par* novi = new Par({kljuc, V()});
            niz[hashirano] = novi;
            return niz[hashirano]->second;
        }

        int iteracije = 0;
        while(niz[hashirano] == DEL || niz[hashirano]->first != kljuc) {
            std::cout << "Kolizija, " << kljuc << " hoce na adresu " << hashirano << " a tamo je " << niz[hashirano]->first << '\n';
            hashirano = (hashirano + 1) % kapacitet;
            if(niz[hashirano] == nullptr) {
                std::cout << "Kreiran " << kljuc << " na adresi " << hashirano << "\n";
                Par* novi = new Par({kljuc, V()});
                niz[hashirano] = novi;
                return niz[hashirano]->second;
            }
            if(++iteracije == kapacitet) {
                std::cout << "MJESTA: " << hashirano << "\n";
                return niz[hashirano]->second;
            }
        }
        return niz[hashirano]->second;
    }
    void obrisi() {
    }
    void obrisi(const K& kljuc) {
        unsigned int hashirano = this->hash(kljuc, kapacitet);
        std::cout << "Brisem " << kljuc << " sa adrese " << hashirano << '\n';
        niz[hashirano] = DEL;
    }
    int brojElemenata() const { return velicina; }
    void ispisi() const {
        for(int i = 0; i < kapacitet; i++) {
            if(niz[i] == nullptr)
                std::cout << i << ": nullptr, none\n";
            else if(niz[i] == DEL)
                std::cout << i << ": DEL, none\n";
            else std::cout << i << ": " << niz[i]->first << ", " << niz[i]->second << "\n";
        }
    }
};


int main() {
    HashMapa<std::string, int> mapa;
    mapa["bakir"] = 3;
    mapa["baki"] = 4;
    mapa["bak"] = 3;
    mapa.ispisi();
    mapa.obrisi("bakir");
    mapa.ispisi();
    std::cout << mapa["baki"] << "\n";
    mapa["bakir"] = 2;
    mapa["gewad"] = 7;
    mapa["gewa"] = 7;
    mapa["gew"] = 7;
    mapa["ge"] = 7;
    mapa["g"] = 7;
    mapa["gewadoloid"] = 7;
    mapa["gewaoloid"] = 7;
    mapa.ispisi();

    return 0;
}
