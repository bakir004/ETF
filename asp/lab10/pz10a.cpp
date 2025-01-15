#include <cstdlib>

template <typename T>
class Cvor {
    int redniBroj;
    T oznaka;
public:
    Cvor(): redniBroj(-1), oznaka(T()) {}
    Cvor(int redniBroj, T oznaka): redniBroj(redniBroj), oznaka(oznaka) {}
    T dajOznaku() { return oznaka; }
    void postaviOznaku(T oznaka) { this->oznaka = oznaka; }
};

template <typename T>
class Grana {
    Cvor<T>* polazni;
    Cvor<T>* dolazni;
    float tezina;
    T oznaka;
public:
    Grana(): polazni(nullptr), dolazni(nullptr), tezina(-1), oznaka(T()) {}
    float dajTezinu() { return tezina; }
    T dajOznaku() { return oznaka; }
    void postaviTezinu(float tezina) { this->tezina = tezina; }
    void postaviOznaku(T oznaka) { this->oznaka = oznaka; }
    Cvor<T>* dajPolazni() { return polazni; }
    Cvor<T>* dajDolazni() { return dolazni; }
};

template <typename T>
class UsmjereniGraf {
    UsmjereniGraf(int brojCvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina = 0) = 0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina = 0) = 0;
    virtual float dajTezinuGrane(int polazni, int dolazni) = 0;
    virtual bool postojiGrana(int polazni, int dolazni) = 0;
    virtual void postaviOznakuCvora(int cvor, const T& oznaka) = 0;
    virtual T dajOznakuCvora(int cvor) = 0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, const T& oznaka) = 0;
    virtual Grana<T>& dajGranu(int polazni, int dolazni) = 0;
    virtual Cvor<T>& dajCvor(int cvor) = 0;
};

template <typename T>
class MatricaGraf : public UsmjereniGraf<T> {
    int brojCvorova;
    Grana<T>** grane;   
    Cvor<T>* cvorovi;
public:
    MatricaGraf<T>(int brojCvorova) : brojCvorova(brojCvorova), grane(new Grana<T>*[brojCvorova]{}) {
        for (int i = 0; i < brojCvorova; i++) {
            grane[i] = new Grana<T>[brojCvorova]{};
            for(int j = 0; j < brojCvorova; j++)
                grane[i][j] = Grana<T>();
        }
    }
    ~MatricaGraf<T>() {
        for (int i = 0; i < brojCvorova; i++)
            delete[] grane[i];
        delete[] grane;
    }
    int dajBrojCvorova() { return brojCvorova; }
    void dodajGranu(int polazni, int dolazni, float tezina = 0) {
        grane[polazni][dolazni].postaviTezinu(tezina);
    }
    void obrisiGranu(int polazni, int dolazni) {
        grane[polazni][dolazni].postaviTezinu(-1);
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina = 0) {
        grane[polazni][dolazni].postaviTezinu(tezina);
    }
    float dajTezinuGrane(int polazni, int dolazni) {
        return grane[polazni][dolazni].dajTezinu();
    }
    bool postojiGrana(int polazni, int dolazni) {
        return grane[polazni][dolazni].dajTezinu() != -1;
    }
};


int main() {

    return 0;
}
