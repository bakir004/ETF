#include <iostream>

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
    void preorder(Cvor* c) {
        if(c == nullptr) return;
        std::cout << c->kljuc << ",";
        preorder(c->lijevo);
        preorder(c->desno);
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
    void obrisi() { brisiOdDatogCvoraNadole(korijen); korijen = nullptr; brojEl = 0; }
    void obrisi(Cvor* c, K i) {
        Cvor *temp = nullptr;

        while (c != nullptr && c->kljuc != i) {
            temp = c;
            if (c->kljuc < i) c = c->desno;
            else c = c->lijevo;
        }
        if (c == nullptr) return;

        Cvor *c1, *c2, *c3;
        if (!c->lijevo){
            c1 = c->desno;
            c3 = temp;
        } else if (!c->desno){
            c1 = c->lijevo;
            c3 = temp;
        } else {
            c1 = c->lijevo;
            c2 = c1->desno;
            c3 = c;
            while (c2) {
                c3 = c1;
                c1 = c2;
                c2 = c1->desno;
            }
            if (c != c3) {
                c3->desno = c1->lijevo;
                if (c1->lijevo)
                    c1->lijevo->roditelj = c3;
                c1->lijevo = c->lijevo;
                if (c->lijevo)
                    c->lijevo->roditelj = c1;
            }
            c1->desno = c->desno;
            if (c->desno)
                c->desno->roditelj = c1;
        }

        if (!temp) korijen = c1;
        else {
            if (temp->lijevo == c) {
                temp->lijevo = c1;
                temp->tezina--;
            } else {
                temp->desno = c1;
                temp->tezina++;
            }
        }

        if (c1) c1->roditelj = temp;

        c->desno = c->lijevo = c->roditelj = nullptr;
        delete c;
        brojEl--;
    }
    void obrisi(const K& kljuc) { obrisi(korijen, kljuc); }
    int brojElemenata() const { return brojEl; }
    void ispisi() const { ispisi(korijen); }
    void Preorder() { preorder(korijen); }
};

int main() {
    AVLStabloMapa<int,int> s1;
    s1[6] = 6;
    s1[1] = 1;
    s1[10] = 10;
    s1[7] = 7;
    s1[3] = 3;
    s1[2] = 2;
    s1[4] = 4;
    s1[9] = 9;
    s1[5] = 5;
    s1[8] = 8;
    s1.Preorder();
    std::cout << "\n";
    AVLStabloMapa<char, char> s2;
    s2['A'] = 'C';
    s2['B'] = 'C';
    s2['C'] = 'C';
    s2['D'] = 'C';
    s2['F'] = 'C';
    s2['G'] = 'C';
    s2['H'] = 'C';
    s2['J'] = 'C';
    s2['K'] = 'C';
    s2['L'] = 'C';
    s2.Preorder();
    std::cout << "\n";
    AVLStabloMapa<double, double> s3;
    s3[4.52] = 0.4;
    s3[3.67] = 0.4;
    s3[2.33] = 0.4;
    s3[1.52] = 0.4;
    s3[9.60] = 0.4;
    s3[5.57] = 0.4;
    s3[4.67] = 0.4;
    s3[2.61] = 0.4;
    s3[3.41] = 0.4;
    s3[8.37] = 0.4;
    s3[4.52] = 0.4;
    s3.Preorder();
    std::cout << "\n";
    AVLStabloMapa<double, int> s4;
    s4[4.52] = 4;
    s4[3.67] = 4;
    s4[8.33] = 4;
    s4[1.52] = 4;
    s4[9.60] = 4;
    s4[18.57] = 4;
    s4[4.67] = 4;
    s4[2.61] = 4;
    s4[15.41] = 4;
    s4[8.37] = 4;
    s4[16.52] = 4;
    s4.Preorder();
    std::cout << "\n";
    AVLStabloMapa<std::string, std::string> s5;
    s5["Bakir"] = "Cinjarevic";
    s5["Vedad"] = "Gastan";
    s5["Amar"] = "Handanagic";
    s5["Emin"] = "Begic";
    s5["Tarik"] = "Lolic";
    s5["Mirza M."] = "Halilovic";
    s5["Omer"] = "Maslesa";
    s5["Semir"] = "Jamakovic";
    s5["Muhamed"] = "Kadric";
    s5["Dzelaludin"] = "Rumi";
    s5.Preorder();
    std::cout << "\n";
    return 0;
}
