#include <iostream>
#include <fstream>
#include <functional>
#include <ctime>
#include <stdexcept>

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


unsigned int hashiranje(int a, unsigned int max) {
    std::cout << "HASHIRANJE\n";
    std::ofstream outFile("example.txt"); // Open a file for writing
    if (outFile.is_open()) {
        outFile << "Hello, this is a test file.\n";
        outFile << "C++ file handling is fun!\n";
        outFile.close(); // Close the file after writing
    } else {
        std::cerr << "Unable to open file for writing.\n";
        return 1;
    }

    std::ifstream inFile("example.txt"); // Open a file for reading
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) { // Read the file line by line
            std::cout << line << '\n'; // Print each line to the console
        }
        inFile.close(); // Close the file after reading
    } else {
        std::cerr << "Unable to open file for reading.\n";
        return 1;
    }
    std::cout << "HASHIRANJE\n";
    return a % max;
}
template <typename K, typename V>
class HashMapa : public Mapa<K, V> {
    typedef std::pair<K, V> Par;
    int velicina;
    int kapacitet;
    Par** niz;
    unsigned int (*hash)(K ulaz, unsigned int max);
    Par* DEL;
    std::pair<int, bool> trazi(int trenutnaAdresa, const K& kljuc) const {
        int prviPronadjenDEL = -1;
        for(int i = 0; i < kapacitet; i++, trenutnaAdresa = (trenutnaAdresa + 1) % kapacitet) {
            // std::cout << "trenutna adresa: " << trenutnaAdresa << "\n";
            // std::cout << "PRVI DEL: " << prviPronadjenDEL << "\n";
            if(niz[trenutnaAdresa] == nullptr && prviPronadjenDEL == -1)
                return {trenutnaAdresa, true};
            if(niz[trenutnaAdresa] == DEL && prviPronadjenDEL == -1)
                prviPronadjenDEL = trenutnaAdresa;
            if(niz[trenutnaAdresa] != nullptr && niz[trenutnaAdresa]->first == kljuc)
                return {trenutnaAdresa, false};
        }
        if(prviPronadjenDEL == -1) {
            throw std::domain_error("PUNA MAPA!");
        }
        return {prviPronadjenDEL, true};
    }
    void dodajNaAdresu(const K& kljuc, int adresa) {
        /*std::cout << "Kreiran " << kljuc << " na adresi " << adresa << "\n";*/
        if(niz[adresa] != DEL && niz[adresa] != nullptr) {
            std::cout << "OVERWRITEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << kljuc << " uzima mjesto " << niz[adresa]->first << "\n";
            delete niz[adresa];
        }
        velicina++;
        niz[adresa] = new Par({kljuc, V()});
    }
public:
    HashMapa(): velicina(0), DEL(new Par({K(),V()})), kapacitet(POCETNA_VELICINA_HASH_TABELE), niz(new Par*[kapacitet]{}) { 
        hashiranje(0,1);
    }
    ~HashMapa() {
        delete DEL;
        for(int i = 0; i < kapacitet; i++)
            if(niz[i] != DEL)
                delete niz[i];
        delete[] niz;
    }
    HashMapa(const HashMapa& nm): velicina(nm.velicina), kapacitet(nm.kapacitet), hash(nm.hash) {
        if (this == &nm)
            return;
        DEL = new Par({K(),V()});
        niz = new Par*[kapacitet]{};
        for (int i = 0; i < kapacitet; i++) {
            if (nm.niz[i] == nm.DEL) 
                niz[i] = DEL;
            else if (nm.niz[i] != nullptr)
                niz[i] = new Par(*nm.niz[i]);
        }
    }
    HashMapa& operator=(const HashMapa& nm) {
        if (this == &nm)
            return *this;
        delete DEL;
        for (int i = 0; i < kapacitet; i++)
            if (niz[i] != nullptr && niz[i] != DEL)
                delete niz[i];
        delete[] niz;

        kapacitet = nm.kapacitet;
        velicina = nm.velicina;
        DEL = new Par({K(), V()});

        niz = new Par*[kapacitet]{};
        for (int i = 0; i < kapacitet; i++) {
            if (nm.niz[i] == nm.DEL) 
                niz[i] = DEL;
            else if (nm.niz[i] != nullptr)
                niz[i] = new Par(*nm.niz[i]);
        }

        hash = nm.hash; 
        return *this;
    }
    V operator[](const K& kljuc) const {
        unsigned int hashirano = this->hash(kljuc, kapacitet);
        std::pair<int,bool> pronadjenaAdresa = trazi(hashirano, kljuc);
        // std::cout << "Pronadjena konstantna adresa: " << pronadjenaAdresa.first << '\n';
        if(niz[pronadjenaAdresa.first] == nullptr) return V();
        return niz[pronadjenaAdresa.first]->second;
    }
    V& operator[](const K& kljuc) {
        unsigned int hashirano = this->hash(kljuc, kapacitet);
        std::pair<int,bool> pronadjenaAdresa = trazi(hashirano, kljuc);
        if(pronadjenaAdresa.second) dodajNaAdresu(kljuc, pronadjenaAdresa.first);
        return niz[pronadjenaAdresa.first]->second;
    }
    void obrisi() {
        for(int i = 0; i < kapacitet; i++) {
            delete niz[i];
            niz[i] = nullptr;
        }
        velicina = 0;
    }
    void obrisi(const K& kljuc) {
        unsigned int hashirano = this->hash(kljuc, kapacitet);
        std::pair<int,bool> pronadjenaAdresa = trazi(hashirano, kljuc);
        if(!pronadjenaAdresa.second) {
            delete niz[pronadjenaAdresa.first];
            niz[pronadjenaAdresa.first] = DEL;
            velicina--;
            return;
        }
        std::cout << "Ne postoji u mapi\n";
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
        std::cout << "\n";
    }
    void definisiHashFunkciju(unsigned int(*funkcija)(K ulaz, unsigned int max)) {
        hash = funkcija;
    }
};


int main() {
    hashiranje(0, 0);

    // NizMapa<int,int> nm;
    // BinStabloMapa<int,int> bm;
    // HashMapa<int,int> hm;
    // hm.definisiHashFunkciju(hashiranje);
    // clock_t vrijeme1 = clock();
    // int brojElemenata = 10000;
    // for(int i = 0; i < brojElemenata; i++)
    //     nm[i] = i;
    // clock_t vrijeme2 = clock();
    // int umetanjeNizMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    // vrijeme1 = clock();
    // for(int i = 0; i < brojElemenata; i++)
    //     bm[i] = i;
    // vrijeme2 = clock();
    // int umetanjeBinStabloMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    // vrijeme1 = clock();
    // for(int i = 0; i < brojElemenata; i++)
    //     hm[i] = i;
    // vrijeme2 = clock();
    // int umetanjeHashMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    // vrijeme1 = clock();
    // for(int i = 0; i < brojElemenata; i++) nm[i];
    // vrijeme2 = clock();
    // int pristupNizMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    // vrijeme1 = clock();
    // for(int i = 0; i < brojElemenata; i++) bm[i];
    // vrijeme2 = clock();
    // int pristupBinStabloMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    // vrijeme1 = clock();
    // for(int i = 0; i < brojElemenata; i++) hm[i];
    // vrijeme2 = clock();
    // int pristupHashMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    HashMapa<int,int> hm;
    hm.definisiHashFunkciju(hashiranje);
    hm[1] = 1;
    hm[2] = 2;
    hm[3] = 3;
    hm[4] = 4;
    hm[5] = 44;
    hm[15] = 44;
    hm[25] = 44;

    hm.ispisi();


    /*NizMapa<int,int> nm;*/
    /*BinStabloMapa<int,int> bm;*/
    /*HashMapa<int,int> hm;*/
    /*hm.definisiHashFunkciju(hashiranje);*/
    /*clock_t vrijeme1 = clock();*/
    /*int brojElemenata = 10000;*/
    /*for(int i = 0; i < brojElemenata; i++)*/
    /*    nm[i] = i;*/
    /*clock_t vrijeme2 = clock();*/
    /*int umetanjeNizMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);*/
    /*vrijeme1 = clock();*/
    /*for(int i = 0; i < brojElemenata; i++)*/
    /*    bm[i] = i;*/
    /*vrijeme2 = clock();*/
    /*int umetanjeBinStabloMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);*/
    /*vrijeme1 = clock();*/
    /*for(int i = 0; i < brojElemenata; i++)*/
    /*    hm[i] = i;*/
    /*vrijeme2 = clock();*/
    /*int umetanjeHashMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);*/
    /*vrijeme1 = clock();*/
    /*for(int i = 0; i < brojElemenata; i++) nm[i];*/
    /*vrijeme2 = clock();*/
    /*int pristupNizMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);*/
    /*vrijeme1 = clock();*/
    /*for(int i = 0; i < brojElemenata; i++) bm[i];*/
    /*vrijeme2 = clock();*/
    /*int pristupBinStabloMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);*/
    /*vrijeme1 = clock();*/
    /*for(int i = 0; i < brojElemenata; i++) hm[i];*/
    /*vrijeme2 = clock();*/
    /*int pristupHashMapa = (vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);*/
    /**/
    /*std::cout << "NizMapa:\n";*/
    /*std::cout << "  Pristup: " << pristupNizMapa << "ms\n";*/
    /*std::cout << "  Umetanje: " << umetanjeNizMapa << "ms\n";*/
    /*std::cout << "BinStabloMapa:\n";*/
    /*std::cout << "  Pristup: " << pristupBinStabloMapa << "ms\n";*/
    /*std::cout << "  Umetanje: " << pristupBinStabloMapa << "ms\n";*/
    /*std::cout << "HashMapa:\n";*/
    /*std::cout << "  Pristup: " << pristupHashMapa << "ms\n";*/
    /*std::cout << "  Umetanje: " << umetanjeHashMapa << "ms\n";*/

    // Primjer ispisa:
    // NizMapa:
    //   Pristup: 124ms 
    //   Umetanje: 260ms
    // BinStabloMapa:   
    //   Pristup: 223ms 
    //   Umetanje: 223ms
    // HashMapa:        
    //   Pristup: 0ms   
    //   Umetanje: 1ms
    //
    // std::cout << "NizMapa:\n";
    // std::cout << "  Pristup: " << pristupNizMapa << "ms\n";
    // std::cout << "  Umetanje: " << umetanjeNizMapa << "ms\n";
    // std::cout << "BinStabloMapa:\n";
    // std::cout << "  Pristup: " << pristupBinStabloMapa << "ms\n";
    // std::cout << "  Umetanje: " << pristupBinStabloMapa << "ms\n";
    // std::cout << "HashMapa:\n";
    // std::cout << "  Pristup: " << pristupHashMapa << "ms\n";
    // std::cout << "  Umetanje: " << umetanjeHashMapa << "ms\n";
    //
    // // Primjer ispisa:
    // // NizMapa:
    // //   Pristup: 124ms 
    // //   Umetanje: 260ms
    // // BinStabloMapa:   
    // //   Pristup: 223ms 
    // //   Umetanje: 223ms
    // // HashMapa:        
    // //   Pristup: 0ms   
    // //   Umetanje: 1ms
    // //
    // // Vidimo da NizMapa pati od umetanja jer se mora prvo linearno pretraziti kljuc, 
    // // i dodati ga ako ga ne nadje. Pristup je brzi od umetanja jer vrlo je moguce naci element prije kraja.
    // //
    // // BinStabloMapa za ovaj konkretan slucaj ce biti linked lista je se svi elementi nadodaju na kraj.
    // // Zbog toga su identicni pristup i umetanje u smislu vremena izvrsavanja. 
    // // Brzinu BinStabloMape nad nasumicnim skupom podataka smo vidjeli u PZ7.
    // //
    // // HashMapa je neobicno brza. Koristi se u implementaciji std::unorderded_map.
    // return 0;
}
