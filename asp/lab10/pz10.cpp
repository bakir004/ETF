#include <iostream>
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
        std::cout << "Posjecen cvor " << pocetniCvor << "\n";
        while(!q.empty()) {
            int trenutniCvor = q.front();
            q.pop();
            for(int i = 0; i < brojCvorova; i++) {
                if(grane[trenutniCvor][i].dajTezinu() != -1 && !posjeceni[i]) {
                    q.push(i);
                    posjeceni[i] = true;
                    std::cout << "Posjecen cvor " << i << "\n";
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


int main() {
    MatricaGraf<int> g(6);
    g.dodajGranu(0, 1);
    g.dodajGranu(1, 0);
    g.dodajGranu(0, 2);
    g.dodajGranu(0, 3);
    g.dodajGranu(2, 5);
    g.dodajGranu(1, 4);
    g.dodajGranu(1, 5);
    g.dfs(0);
    g.bfs(0);

    return 0;
}
