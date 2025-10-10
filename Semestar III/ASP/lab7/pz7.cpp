#include <ctime>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <vector>

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

template <typename K, typename V>
class BinStabloMapa: public Mapa<K,V> {
    struct Cvor {
        K kljuc;
        V vrijednost;
        Cvor* lijevo;
        Cvor* desno;
        Cvor* roditelj;
        int tezina;
    };
    Cvor* korijen;
    int brojEl;
    void brisiOdDatogCvoraNadole(Cvor* c) {
        if(c == nullptr)
            return;
        Cvor* lijevi = c->lijevo;
        Cvor* desni = c->desno;
        delete c;
        brisiOdDatogCvoraNadole(lijevi);
        brisiOdDatogCvoraNadole(desni);
    }
    void ispisi(Cvor* c) const { 
        if(c == nullptr) { return; }
        std::cout << "(" << c->kljuc << ", " << c->vrijednost << ", " << c->tezina << ")\n";
        if(c->lijevo != nullptr)
            std::cout << "Lijevo od " << c->kljuc << ": " << c->lijevo->kljuc << "\n";
        if(c->desno != nullptr)
            std::cout << "Desno od " << c->kljuc << ": " << c->desno->kljuc << "\n";
        ispisi(c->lijevo);
        ispisi(c->desno);
    }
public:
    BinStabloMapa() : korijen(nullptr), brojEl(0) {}
    ~BinStabloMapa() { obrisi(); }
    BinStabloMapa(const BinStabloMapa& b) : korijen(nullptr), brojEl(0) {
        std::function<Cvor*(Cvor*)> kopiraj = [&](Cvor* c) -> Cvor* {
            if (c == nullptr) return nullptr;
            Cvor* noviCvor = new Cvor{c->kljuc, c->vrijednost, nullptr, nullptr, nullptr, c->tezina};
            noviCvor->lijevo = kopiraj(c->lijevo);
            noviCvor->desno = kopiraj(c->desno);
            if (noviCvor->lijevo) noviCvor->lijevo->roditelj = noviCvor;
            if (noviCvor->desno) noviCvor->desno->roditelj = noviCvor;
            return noviCvor;
        };

        korijen = kopiraj(b.korijen);
        brojEl = b.brojEl;
    }
    BinStabloMapa& operator=(const BinStabloMapa& b) {
        if (this != &b) {
            obrisi();

            std::function<Cvor*(Cvor*)> kopiraj = [&](Cvor* c) -> Cvor* {
                if (c == nullptr) return nullptr;
                Cvor* noviCvor = new Cvor{c->kljuc, c->vrijednost, nullptr, nullptr, nullptr, c->tezina};
                noviCvor->lijevo = kopiraj(c->lijevo);
                noviCvor->desno = kopiraj(c->desno);
                if (noviCvor->lijevo) noviCvor->lijevo->roditelj = noviCvor;
                if (noviCvor->desno) noviCvor->desno->roditelj = noviCvor;
                return noviCvor;
            };

            korijen = kopiraj(b.korijen);
            brojEl = b.brojEl;
        }
        return *this;
    }
    V operator[](const K& kljuc) const {
        Cvor* temp = korijen;
        while (temp != nullptr) {
            if (kljuc == temp->kljuc)
                return temp->vrijednost;
            else if (kljuc < temp->kljuc)
                temp = temp->lijevo;
            else
                temp = temp->desno;
        }
        return V();
    }
    V& operator[](const K& kljuc) {
        if(korijen == nullptr) {
            korijen = new Cvor{kljuc, V(), nullptr, nullptr, nullptr, 0};
            brojEl++;
            return korijen->vrijednost;
        }
        Cvor* temp = korijen;
        Cvor* roditelj = nullptr;
        bool stavljamDesno = false;
        while (temp != nullptr) {
            if (kljuc == temp->kljuc) {
                return temp->vrijednost;
            }
            else if (kljuc < temp->kljuc) {
                roditelj = temp;
                temp = temp->lijevo;
                stavljamDesno = false;
            } else {
                roditelj = temp;
                temp = temp->desno;
                stavljamDesno = true;
            }
        }
        temp = new Cvor{kljuc, V(), nullptr, nullptr, roditelj, 0};
        if(!stavljamDesno) roditelj->lijevo = temp;
        else roditelj->desno = temp;
        brojEl++;
        return temp->vrijednost;
    }
    void obrisi() {
        brisiOdDatogCvoraNadole(korijen); korijen = nullptr; brojEl = 0;
    }
    void obrisi(const K& k) {
        auto p = korijen;
        Cvor* roditelj = nullptr;
        while(p != nullptr && k != p->kljuc) {
            roditelj = p;
            if(k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }
        if(p == nullptr) return;
        Cvor* m;
        if(p->lijevo == nullptr) {
            m = p->desno;
        } else {
            auto pm = p;
            m = p->lijevo;
            auto temp = m->desno;
            while(temp != nullptr) {
                pm = m;
                m = temp;
                temp = m->desno;
            }
            if(pm != p) {
                pm->desno = m->lijevo;
                m->lijevo = p->lijevo;
            }
            m->desno = p->desno;
        }
        if(roditelj == nullptr)
            korijen = m;
        else {
            if(p == roditelj->lijevo) roditelj->lijevo = m;
            else roditelj->desno = m;
        }
        delete p;
        brojEl--;
    }
    int brojElemenata() const { return brojEl; }
    void ispisi() const { ispisi(korijen); }
};

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));
    const int brojUzoraka = 30000;
    const int min = 1, max = 10000;
    std::vector<int> uzorci;
    for(int i = 0; i < brojUzoraka; i++) {
        int random = std::rand() % (max - min + 1) + min;
        uzorci.push_back(random);
    }

    NizMapa<int, int> m1;
    BinStabloMapa<int, int> m2;

    clock_t vrijeme1 = clock();
    for(auto e : uzorci)
        m1[e] = e;
    clock_t vrijeme2 = clock();
    int umetanjeNiz = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);

    vrijeme1 = clock();
    for(auto e : uzorci)
        m2[e] = e;
    vrijeme2 = clock();
    int umetanjeBinarno = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);

    int s = 0;
    vrijeme1 = clock();
    for(auto e : uzorci)
        s += m1[e];
    vrijeme2 = clock();
    int pristupNiz = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);

    s = 0;
    vrijeme1 = clock();
    for(auto e : uzorci)
        s += m2[e];
    vrijeme2 = clock();
    int pristupBinarno = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);

    std::cout << "Umetanje u NizMapa: " << umetanjeNiz << "ms\n";
    std::cout << "Umetanje u BinStabloMapa: " << umetanjeBinarno << "ms\n";
    std::cout << "Pristup NizMapa: " << pristupNiz << "ms\n";
    std::cout << "Pristup BinStabloMapa: " << pristupBinarno << "ms\n";

    // Uzorak od 30000 nasumicnih intova, u opsegu od 1 do 10000.
    // Za moj procesor AMD Ryzen 5 2600, rezultati su slijedeci:
    //
    // Umetanje u NizMapa: 429ms    
    // Umetanje u BinStabloMapa: 5ms
    // Pristup NizMapa: 307ms       
    // Pristup BinStabloMapa: 4ms
    //
    // Rezultati su ocekivani: umetanje u NizMapu je najskuplje
    // sa kompleksnosti O(n) gdje je n broj elemenata.
    // Isto sa pristupom NizMapi, jer mora pretrazit svaki element
    // dok ne nadje svoj kljuc. Umetanje i pristup BinStabloMapi 
    // je mnogo bolje, zbog svoje logaritamske kompleknosti.

    return 0;
}
