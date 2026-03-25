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
    int dajVisinu(Cvor* c) {
        if(c == nullptr) return -1;
        return 1 + std::max(dajVisinu(c->lijevo), dajVisinu(c->desno));
    }
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
    void lijevaRotacija(Cvor* c) {
        /*std::cout << "ROTIRAM LIJEVO OKO " << c->kljuc << "\n";*/
        Cvor* babo = c->roditelj;
        Cvor* dedo = babo->roditelj;

        babo->desno = c->lijevo;
        if (c->lijevo != nullptr)
            c->lijevo->roditelj = babo;

        c->lijevo = babo;
        babo->roditelj = c;
        c->roditelj = dedo;

        if (dedo != nullptr) {
            if (dedo->lijevo == babo)
                dedo->lijevo = c;
            else
                dedo->desno = c;
        } else {
            korijen = c;
        }

        babo->tezina = dajVisinu(babo->lijevo) - dajVisinu(babo->desno);
        c->tezina = dajVisinu(c->lijevo) - dajVisinu(c->desno);
    }

    void desnaRotacija(Cvor* c) {
        /*std::cout << "ROTIRAM DESNO OKO " << c->kljuc << "\n";*/
        Cvor* babo = c->roditelj;
        Cvor* dedo = babo->roditelj;

        babo->lijevo = c->desno;
        if (c->desno != nullptr)
            c->desno->roditelj = babo;

        c->desno = babo;
        babo->roditelj = c;
        c->roditelj = dedo;

        if (dedo != nullptr) {
            if (dedo->lijevo == babo)
                dedo->lijevo = c;
            else
                dedo->desno = c;
        } else {
            korijen = c;
        }

        babo->tezina = dajVisinu(babo->lijevo) - dajVisinu(babo->desno);
        c->tezina = dajVisinu(c->lijevo) - dajVisinu(c->desno);
    }
    void ispisiCvor(Cvor* c) {
        if(c == nullptr) std::cout << "nullptr\n";
        std::cout << "Cvor (" << c->kljuc << ", " << c->vrijednost << ", " << c->tezina << ")\n";
        std::cout << "Lijevo: " << (c->lijevo ? c->lijevo->kljuc : -1) << "\n";
        std::cout << "Desno: " << (c->desno ? c->desno->kljuc : -1) << "\n";
    }
public:
    BinStabloMapa() : korijen(nullptr), brojEl(0) {}
    ~BinStabloMapa() { obrisi(); }
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
            if (kljuc == temp->kljuc)
                return temp->vrijednost;
            else if (kljuc < temp->kljuc) {
                roditelj = temp;
                roditelj->tezina--;
                temp = temp->lijevo;
                stavljamDesno = false;
            } else {
                roditelj = temp;
                roditelj->tezina++;
                temp = temp->desno;
                stavljamDesno = true;
            }
        }
        temp = new Cvor{kljuc, V(), nullptr, nullptr, roditelj, 0};
        if(!stavljamDesno) roditelj->lijevo = temp;
        else roditelj->desno = temp;
        brojEl++;
        Cvor* c = temp->roditelj;
        while(c != nullptr) {
            if(c->tezina >= 2) {
                if(c->desno->tezina == -1)
                    desnaRotacija(c->desno->lijevo);
                lijevaRotacija(c->desno);
                break;
            } else if(c->tezina <= -2) {
                if(c->lijevo->tezina == 1)
                    lijevaRotacija(c->lijevo->desno);
                desnaRotacija(c->lijevo);
                break;
            }
            c = c->roditelj;
        }
        return temp->vrijednost;
    }
    void obrisi() {brisiOdDatogCvoraNadole(korijen); korijen = nullptr; brojEl = 0;}
    void obrisi(const K& kljuc) {}
    int brojElemenata() const { return brojEl; }
    void ispisi() const { ispisi(korijen); }
};

int main() {
    BinStabloMapa<int, int> m1;
    m1[1] = 2;
    m1[5] = 22;
    m1[6] = -3;
    m1[4] = 6;
    m1[3] = 2;
    m1[2] = 5;
    m1[8] = 1;
    m1[0] = 1;
    m1[7] = 1;
    m1.ispisi();

    return 0;
}
