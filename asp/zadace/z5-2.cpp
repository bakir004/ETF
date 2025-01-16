#include <iostream>
#include <list>
#include <stdexcept>
#include <queue>
#include <stack>

using namespace std;

template <typename T>
class Cvor {
    int redniBroj;
    T oznaka;
public:
    Cvor(): redniBroj(0), oznaka(T()) {}
    Cvor(int redniBroj, T oznaka): redniBroj(redniBroj), oznaka(oznaka) {}
    int dajRedniBroj() { return redniBroj; }
    T dajOznaku() { return oznaka; }
    void postaviOznaku(T oznaka) { this->oznaka = oznaka; }
};

template <typename T>
class Grana {
    Cvor<T> polazni;
    Cvor<T> dolazni;
    float tezina;
    T oznaka;
public:
    Grana(): polazni(Cvor<T>()), dolazni(Cvor<T>()), tezina(-1), oznaka(T()) {}
    Grana(Cvor<T> polazni, Cvor<T> dolazni, float tezina = 0, T oznaka = T()): polazni(polazni), dolazni(dolazni), tezina(tezina), oznaka(oznaka) {}
    float dajTezinu() { return tezina; }
    T dajOznaku() { return oznaka; }
    void postaviTezinu(float tezina) { this->tezina = tezina; }
    void postaviOznaku(T oznaka) { this->oznaka = oznaka; }
    Cvor<T> dajPolazniCvor() { return polazni; }
    Cvor<T> dajDolazniCvor() { return dolazni; }
};

template <typename T>
class GranaIterator;


template <typename T>
class UsmjereniGraf {
public:
    UsmjereniGraf(int brojCvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina = 0) = 0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina = 0) = 0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const = 0;
    virtual bool postojiGrana(int polazni, int dolazni) const = 0;
    virtual void postaviOznakuCvora(int cvor, const T& oznaka) = 0;
    virtual T dajOznakuCvora(int cvor) const = 0;
    virtual T dajOznakuGrane(int polazna, int dolazna) const = 0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, const T& oznaka) = 0;
    virtual Grana<T>& dajGranu(int polazni, int dolazni) const = 0;
    virtual Cvor<T>& dajCvor(int cvor) const = 0;
    virtual GranaIterator<T> dajGranePocetak() = 0;
    virtual GranaIterator<T> dajGraneKraj() = 0;
    virtual void ispisiCvorove() = 0;
};

template <typename T>
class GranaIterator {
    const UsmjereniGraf<T> &graf;
    int redniBrojGrane;
public:
    GranaIterator<T>(const UsmjereniGraf<T>& g, int redniBrojGrane): graf(g), redniBrojGrane(redniBrojGrane) {}
    GranaIterator<T>(const UsmjereniGraf<T>& g): redniBrojGrane(0), graf(g) {}
    Grana<T> operator*() {
        if(redniBrojGrane >= graf.dajBrojCvorova() * graf.dajBrojCvorova())
            throw std::domain_error("Iterator je izvan opsega"); 
        return graf.dajGranu(redniBrojGrane / graf.dajBrojCvorova(), redniBrojGrane % graf.dajBrojCvorova());
    }
    bool operator==(const GranaIterator &iter) const {
        return redniBrojGrane == iter.redniBrojGrane;
    }
    bool operator!=(const GranaIterator &iter) const {
        return redniBrojGrane != iter.redniBrojGrane;
    }
    GranaIterator& operator++() {
        redniBrojGrane++;
        while(!graf.postojiGrana(redniBrojGrane / graf.dajBrojCvorova(), redniBrojGrane % graf.dajBrojCvorova())) {
            redniBrojGrane++;
            if(redniBrojGrane >= graf.dajBrojCvorova() * graf.dajBrojCvorova())
                break;
        }
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator<T> temp = *this;
        redniBrojGrane++;
        return temp;
    }
};

template <typename T>
class MatricaGraf : public UsmjereniGraf<T> {
    int brojCvorova;
    Grana<T>** grane;   
    Cvor<T>* cvorovi;
public:
    MatricaGraf<T>(int brojCvorova) : UsmjereniGraf<T>(brojCvorova), brojCvorova(brojCvorova), grane(new Grana<T>*[brojCvorova]) {
        for (int i = 0; i < brojCvorova; i++) {
            grane[i] = new Grana<T>[brojCvorova]{};
            for(int j = 0; j < brojCvorova; j++)
                grane[i][j] = Grana<T>();
        }
        cvorovi = new Cvor<T>[brojCvorova];
    }
    ~MatricaGraf<T>() {
        for (int i = 0; i < brojCvorova; i++)
            delete[] grane[i];
        delete[] grane;
        delete[] cvorovi;
    }
    int dajBrojCvorova() const { return brojCvorova; }
    void dodajGranu(int polazni, int dolazni, float tezina = 0) {
        if(cvorovi[polazni].dajOznaku() == T())
            cvorovi[polazni] = Cvor<T>(polazni, T());
        if(cvorovi[dolazni].dajOznaku() == T())
            cvorovi[dolazni] = Cvor<T>(dolazni, T());
        Grana<T> grana = Grana<T>(cvorovi[polazni], cvorovi[dolazni], tezina);
        grane[polazni][dolazni] = grana;
    }
    void obrisiGranu(int polazni, int dolazni) {
        grane[polazni][dolazni].postaviTezinu(-1);
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina = 0) {
        grane[polazni][dolazni].postaviTezinu(tezina);
    }
    float dajTezinuGrane(int polazni, int dolazni) const {
        return grane[polazni][dolazni].dajTezinu();
    }
    bool postojiGrana(int polazni, int dolazni) const {
        return grane[polazni][dolazni].dajTezinu() != -1;
    }
    T dajOznakuCvora(int cvor) const {
        return cvorovi[cvor].dajOznaku();
    }
    T dajOznakuGrane(int polazna, int dolazna) const {
        return grane[polazna][dolazna].dajOznaku();
    }
    void postaviOznakuCvora(int cvor, const T& oznaka) {
        cvorovi[cvor].postaviOznaku(oznaka);
    }
    void postaviOznakuGrane(int polazni, int dolazni, const T& oznaka) {
        grane[polazni][dolazni].postaviOznaku(oznaka);
    }
    Grana<T>& dajGranu(int polazni, int dolazni) const {
        return grane[polazni][dolazni];
    }
    Cvor<T>& dajCvor(int cvor) const {
        return cvorovi[cvor];
    }
    void postaviBrojCvorova(int brojCvorova) {
        this->brojCvorova = brojCvorova;
    }
    GranaIterator<T> dajGranePocetak() {
        return GranaIterator<T>(*this);
    }
    GranaIterator<T> dajGraneKraj() {
        return GranaIterator<T>(*this, brojCvorova * brojCvorova);
    }
    void ispisiCvorove() {
        for(int i = 0; i < brojCvorova; i++)
            cout << cvorovi[i].dajOznaku() << " ";
    }
    void bfs(int pocetniCvor) {
        bool* posjeceni = new bool[brojCvorova];
        for(int i = 0; i < brojCvorova; i++)
            posjeceni[i] = false;

        // ovdje bih koristio vec implementirani Red iz PZ4, 
        // ali zbog glomaznog koda koji nosi sa sobom,
        // koristicu standardni red
        std::queue<int> q;

        q.push(pocetniCvor);
        posjeceni[pocetniCvor] = true;
        while(!q.empty()) {
            int trenutniCvor = q.front();
            std::cout << "Posjecen cvor " << trenutniCvor << "\n";
            q.pop();
            for(int i = 0; i < brojCvorova; i++) {
                if(grane[trenutniCvor][i].dajTezinu() != -1 && !posjeceni[i]) {
                    q.push(i);
                    posjeceni[i] = true;
                }
            }
        }
        delete[] posjeceni;
    }
    void dfs(int pocetniCvor) {
        bool* posjeceni = new bool[brojCvorova];
        for(int i = 0; i < brojCvorova; i++)
            posjeceni[i] = false;

        // ovdje bih koristio vec implementirani Stek iz PZ4, 
        // ali zbog glomaznog koda koji nosi sa sobom,
        // koristicu standardni stack 
        std::stack<int> s;

        s.push(pocetniCvor);
        posjeceni[pocetniCvor] = true;
        while(!s.empty()) {
            int trenutniCvor = s.top();
            std::cout << "Posjecen cvor " << trenutniCvor << "\n";
            s.pop();
            for(int i = 0; i < brojCvorova; i++) {
                if(grane[trenutniCvor][i].dajTezinu() != -1 && !posjeceni[i]) {
                    s.push(i);
                    posjeceni[i] = true;
                }
            }
        }
        delete[] posjeceni;
    }
    
};

template <typename T>
class ListaGraf : public UsmjereniGraf<T> {
public:
    struct LinkedCvor {
        Cvor<T> cvor;
        Grana<T> grana;
        LinkedCvor* sljedeci;
    };
private:
    int brojCvorova;
    LinkedCvor** listaCvorova;
    Cvor<T>* cvoroviInfo;
public:
    ListaGraf<T>(int brojCvorova) : UsmjereniGraf<T>(brojCvorova), brojCvorova(brojCvorova) {
        listaCvorova = new LinkedCvor*[brojCvorova]{};
        cvoroviInfo = new Cvor<T>[brojCvorova];
        for(int i = 0; i < brojCvorova; i++)
            cvoroviInfo[i] = Cvor<T>(i, T());
    }
    ~ListaGraf<T>() {
        for(int i = 0; i < brojCvorova; i++) {
            if(listaCvorova[i] != nullptr) {
                LinkedCvor* trenutni = listaCvorova[i];
                while(trenutni != nullptr) {
                    LinkedCvor* stari = trenutni;
                    trenutni = trenutni->sljedeci;
                    delete stari;
                }
            }
        }
        delete[] listaCvorova;
        delete[] cvoroviInfo;
    }
    int dajBrojCvorova() const { return brojCvorova; }
    void dodajGranu(int polazni, int dolazni, float tezina = 0) {
        LinkedCvor* trenutni = listaCvorova[polazni];
        LinkedCvor* temp = listaCvorova[polazni];
        if(trenutni == nullptr) {
            listaCvorova[polazni] = new LinkedCvor{{dolazni, T()}, {{polazni, T()},{dolazni, T()},tezina, T()}, nullptr};
            return;
        }
        while(trenutni != nullptr) {
            temp = trenutni;
            trenutni = trenutni->sljedeci;
        }
        
        trenutni = new LinkedCvor{{dolazni, T()}, {{polazni, T()},{dolazni, T()},tezina, T()}, nullptr};
        temp->sljedeci = trenutni;

        // std::cout << "trenutni cvor:\n";
        // std::cout << "  " << polazni << "\n";
        // std::cout << "  " << trenutni->cvor.dajRedniBroj() << "\n";
        // std::cout << "  " << trenutni->tezina << "\n";
        // std::cout << "dodo granu " << polazni << "->" << dolazni << "\n";
    }
    void obrisiGranu(int polazni, int dolazni) {
        LinkedCvor* trenutni = listaCvorova[polazni];
        if(trenutni == nullptr) return;
        if(trenutni->cvor.dajRedniBroj() == dolazni) {
            LinkedCvor* krajnji = trenutni->sljedeci;
            delete trenutni;
            listaCvorova[polazni] = krajnji;
            return;
        }

        while(trenutni->sljedeci != nullptr) {
            if(trenutni->sljedeci->cvor.dajRedniBroj() == dolazni) {
                LinkedCvor* krajnji = trenutni->sljedeci->sljedeci;
                delete trenutni->sljedeci;
                trenutni->sljedeci = krajnji;
                return;
            }
            trenutni = trenutni->sljedeci;
        }
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina = 0) { 
        LinkedCvor* trenutni = listaCvorova[polazni];
        while(true) {
            if(trenutni == nullptr) return;
            if(trenutni->cvor.dajRedniBroj() == dolazni)
                trenutni->grana.postaviTezinu(tezina);
            trenutni = trenutni->sljedeci;
        }
    }
    float dajTezinuGrane(int polazni, int dolazni) const { 
        LinkedCvor* trenutni = listaCvorova[polazni];
        while(true) {
            if(trenutni == nullptr) return 0;
            if(trenutni->cvor.dajRedniBroj() == dolazni)
                return trenutni->grana.dajTezinu();
            trenutni = trenutni->sljedeci;
        }
    }
    bool postojiGrana(int polazni, int dolazni) const { 
        LinkedCvor* trenutni = listaCvorova[polazni];
        while(true) {
            if(trenutni == nullptr) return false;
            if(trenutni->cvor.dajRedniBroj() == dolazni) return true;
            trenutni = trenutni->sljedeci;
        }
    }
    T dajOznakuCvora(int cvor) const { 
        return cvoroviInfo[cvor].dajOznaku();
    }
    T dajOznakuGrane(int polazna, int dolazna) const { 
        LinkedCvor* trenutni = listaCvorova[polazna];
        while(true) {
            if(trenutni == nullptr) return 0;
            if(trenutni->cvor.dajRedniBroj() == dolazna)
                return trenutni->grana.dajOznaku();
            trenutni = trenutni->sljedeci;
        }
    }
    void postaviOznakuCvora(int cvor, const T& oznaka) {
        cvoroviInfo[cvor].postaviOznaku(oznaka);
    }
    void postaviOznakuGrane(int polazni, int dolazni, const T& oznaka) { 
        LinkedCvor* trenutni = listaCvorova[polazni];
        while(true) {
            if(trenutni == nullptr) return;
            if(trenutni->cvor.dajRedniBroj() == dolazni)
                trenutni->grana.postaviOznaku(oznaka);
            trenutni = trenutni->sljedeci;
        }
    }
    Grana<T>& dajGranu(int polazni, int dolazni) const { 
        LinkedCvor* trenutni = listaCvorova[polazni];
        while(true) {
            if(trenutni->cvor.dajRedniBroj() == dolazni)
                return trenutni->grana;
            trenutni = trenutni->sljedeci;
        }
    }
    Cvor<T>& dajCvor(int cvor) const {
        return cvoroviInfo[cvor];
    }
    void postaviBrojCvorova(int brojCvorova) {
        this->brojCvorova = brojCvorova;
    }
    GranaIterator<T> dajGranePocetak() {
        return GranaIterator<T>(*this);
    }
    GranaIterator<T> dajGraneKraj() {
        return GranaIterator<T>(*this, brojCvorova * brojCvorova);
    }
    void ispisiCvorove() {
        for(int i = 0; i < brojCvorova; i++) {
            std::cout << i << ": ";
            if(listaCvorova[i] != nullptr) {
                LinkedCvor* trenutni = listaCvorova[i];
                while(trenutni != nullptr) {
                    std::cout << trenutni->cvor.dajRedniBroj() << " ";
                    trenutni = trenutni->sljedeci;
                }
            }
            std::cout << "\n";
        }
    }
    LinkedCvor* dajListuCvorova(int cvor) { return listaCvorova[cvor]; }
};

template <typename T>
void bfs(UsmjereniGraf<T>* g, std::vector<Cvor<T>> v, Cvor<T> pocetni) {
    int brojCvorova = g->dajBrojCvorova();
    bool* posjeceni = new bool[brojCvorova];
    for(int i = 0; i < brojCvorova; i++)
        posjeceni[i] = false;

    // ovdje bih koristio vec implementirani Red iz PZ4, 
    // ali zbog glomaznog koda koji nosi sa sobom,
    // koristicu standardni red
    std::queue<int> q;
    int pocetniCvor = pocetni.dajRedniBroj();
    q.push(pocetniCvor);
    posjeceni[pocetniCvor] = true;
    while(!q.empty()) {
        int trenutniCvor = q.front();
        std::cout << "Posjecen cvor " << trenutniCvor << "\n";
        v.push_back({trenutniCvor, T()});
        q.pop();
        auto trenutni = g->dajListuCvorova(trenutniCvor);
        while(trenutni != nullptr) {
            if(!posjeceni[trenutni->cvor.dajRedniBroj()]) {
                q.push(trenutni->cvor.dajRedniBroj());
                posjeceni[trenutni->cvor.dajRedniBroj()];
            }
            trenutni = trenutni->sljedeci;
        }
    }
    delete[] posjeceni;
}

template <typename T>
void dfs(UsmjereniGraf<T>* g, std::vector<Cvor<T>> v, Cvor<T> pocetni) {
    int brojCvorova = g->dajBrojCvorova();
    bool* posjeceni = new bool[brojCvorova];
    for(int i = 0; i < brojCvorova; i++)
        posjeceni[i] = false;

    // ovdje bih koristio vec implementirani Stek iz PZ4, 
    // ali zbog glomaznog koda koji nosi sa sobom,
    // koristicu standardni stack 
    std::stack<int> s;

    int pocetniCvor = pocetni.dajRedniBroj();
    s.push(pocetniCvor);
    posjeceni[pocetniCvor] = true;
    while(!s.empty()) {
        int trenutniCvor = s.top();
        std::cout << "Posjecen cvor " << trenutniCvor << "\n";
        v.push_back({trenutniCvor, T()});
        s.pop();
        auto trenutni = g->dajListuCvorova(trenutniCvor);
        while(trenutni != nullptr) {
            if(!posjeceni[trenutni->cvor.dajRedniBroj()]) {
                s.push(trenutni->cvor.dajRedniBroj());
                posjeceni[trenutni->cvor.dajRedniBroj()];
            }
            trenutni = trenutni->sljedeci;
        }
    }
    delete[] posjeceni;
}

int main() {
    UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > dfs_obilazak;
    dfs(g, dfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < dfs_obilazak.size(); i++)
        cout << dfs_obilazak[i].dajRedniBroj() << ",";
    delete g;
        return 0;
}
