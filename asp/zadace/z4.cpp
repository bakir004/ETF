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
class AVLStabloMapa: public Mapa<K,V> {
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
    Cvor* kopirajPodstablo(Cvor* original, Cvor* roditelj) {
        if (original == nullptr) return nullptr;

        Cvor* novi = new Cvor{original->kljuc, original->vrijednost, nullptr, nullptr, roditelj, original->tezina};

        novi->lijevo = kopirajPodstablo(original->lijevo, novi);
        novi->desno = kopirajPodstablo(original->desno, novi);

        return novi;
    }
public:
    AVLStabloMapa() : korijen(nullptr), brojEl(0) {}
    ~AVLStabloMapa() { obrisi(); }
    AVLStabloMapa(const AVLStabloMapa& other) : korijen(nullptr), brojEl(0) {
        if (other.korijen != nullptr) {
            korijen = kopirajPodstablo(other.korijen, nullptr);
            brojEl = other.brojEl;
        }
    }
    AVLStabloMapa& operator=(const AVLStabloMapa& other) {
        if (this == &other) return *this; // Handle self-assignment

        obrisi();

        if (other.korijen != nullptr) {
            korijen = kopirajPodstablo(other.korijen, nullptr);
            brojEl = other.brojEl;
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
    void obrisi(const K& kljuc) {
        Cvor* c = korijen;
        Cvor* roditelj = nullptr;

        while (c != nullptr && c->kljuc != kljuc) {
            roditelj = c;
            if (kljuc < c->kljuc) c = c->lijevo;
            else c = c->desno;
        }

        if (c == nullptr) return;

        if (c->lijevo == nullptr || c->desno == nullptr) {
            Cvor* dijete = (c->lijevo != nullptr) ? c->lijevo : c->desno;

            if (c == korijen) {
                korijen = dijete;
            } else {
                if (c == roditelj->lijevo) roditelj->lijevo = dijete;
                else roditelj->desno = dijete;
            }

            if (dijete != nullptr) dijete->roditelj = roditelj;

            delete c;
        } else {
            Cvor* nasljednik = c->desno;
            while (nasljednik->lijevo != nullptr) nasljednik = nasljednik->lijevo;

            c->kljuc = nasljednik->kljuc;
            c->vrijednost = nasljednik->vrijednost;

            obrisi(nasljednik->kljuc);
        }

        Cvor* balansCvor = roditelj;
        while (balansCvor != nullptr) {
            balansCvor->tezina = dajVisinu(balansCvor->lijevo) - dajVisinu(balansCvor->desno);

            if (balansCvor->tezina == 2) {
                if (balansCvor->lijevo->tezina < 0) lijevaRotacija(balansCvor->lijevo->desno);
                desnaRotacija(balansCvor->lijevo);
            } else if (balansCvor->tezina == -2) {
                if (balansCvor->desno->tezina > 0) desnaRotacija(balansCvor->desno->lijevo);
                lijevaRotacija(balansCvor->desno);
            }

            balansCvor = balansCvor->roditelj;
        }

        brojEl--;
    }
    int brojElemenata() const { return brojEl; }
    void ispisi() const { ispisi(korijen); }
    void Preorder() { preorder(korijen); }
    void preorder(Cvor* c) {
        if(c == nullptr) return;
        std::cout << c->kljuc << ",";
        preorder(c->lijevo);
        preorder(c->desno);
    }
};

int main() {
    AVLStabloMapa<int, int> avl;
    avl[1] = 3;
    avl[2] = 3;
    avl[3] = 3;
    avl[4] = 3;
    avl[5] = 3;
    avl[6] = 3;
    avl[7] = 3;
    avl.Preorder();
}
