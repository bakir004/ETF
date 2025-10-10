#include<iostream>
#include<vector>

using namespace std;

template <typename T>
class Lista {
public:
    Lista(){}
    virtual int brojElemenata() const = 0;
    virtual T& trenutni() const = 0;
    virtual bool sljedeci() = 0;
    virtual bool prethodni() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const T& el) = 0;
    virtual void dodajIza(const T& el) = 0;
    virtual T& operator[](int index) const = 0;
    virtual ~Lista() {}
};

template <typename T>
class JednostrukaLista : public Lista<T> {
    struct Cvor {
        Cvor* sljedeci;
        T element;
    };
    int velicina;
    Cvor *trenutniCvor, *pocetniCvor;
    void testPraznine() const { if(velicina == 0) throw "Prazan"; };
public:
    JednostrukaLista(): velicina(0), trenutniCvor(nullptr), pocetniCvor(nullptr) {}
    JednostrukaLista(const JednostrukaLista& lista): velicina(0) {
        Cvor* pomocni = lista.pocetniCvor;
        while(pomocni != nullptr) {
            this->dodajIza(pomocni->element);
            pomocni = pomocni->sljedeci;
            sljedeci();
        }
    }
    JednostrukaLista& operator=(const JednostrukaLista& lista) {
        Cvor* pomocni = pocetniCvor;
        while(pomocni != nullptr) {
            Cvor* trenutni = pomocni;
            pomocni = pomocni->sljedeci;
            delete trenutni;
        }
        velicina = 0;
        pocetniCvor = nullptr;
        trenutniCvor = nullptr;
        pomocni = lista.pocetniCvor;
        while(pomocni != nullptr) {
            this->dodajIza(pomocni->element);
            pomocni = pomocni->sljedeci;
            sljedeci();
        }
        return *this;
    }
    int brojElemenata() const override { return velicina; }
    T& trenutni() const override { testPraznine(); return trenutniCvor->element; }
    bool sljedeci() override;
    bool prethodni() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const T& el) override;
    void dodajIza(const T& el) override;
    T& operator[](int index) const override;
    void ispisi();
    ~JednostrukaLista() {
        Cvor* pomocni = pocetniCvor;
        while(pomocni != nullptr) {
            Cvor* trenutni = pomocni;
            pomocni = pomocni->sljedeci;
            delete trenutni;
        }
        pocetniCvor = nullptr;
        trenutniCvor = nullptr;
        velicina = 0;
    }
};

template <typename T>
void JednostrukaLista<T>::kraj() {
    Cvor* pomocni = pocetniCvor;
    while(pomocni->sljedeci != nullptr)
        pomocni = pomocni->sljedeci;
    trenutniCvor = pomocni;
}

template <typename T>
void JednostrukaLista<T>::pocetak() {
    testPraznine();
    trenutniCvor = pocetniCvor;
}

template <typename T>
bool JednostrukaLista<T>::prethodni() {
    testPraznine();
    if(trenutniCvor == pocetniCvor)
        return false;
    Cvor* pomocni = pocetniCvor;
    while(pomocni->sljedeci != trenutniCvor)
        pomocni = pomocni->sljedeci;
    trenutniCvor = pomocni;
    return true;
}

template <typename T>
bool JednostrukaLista<T>::sljedeci() {
    testPraznine();
    if(trenutniCvor->sljedeci == nullptr) return false;
    trenutniCvor = trenutniCvor->sljedeci;
    return true;
}

template <typename T>
void JednostrukaLista<T>::obrisi() {
    testPraznine();
    if(pocetniCvor == trenutniCvor) {
        pocetniCvor = pocetniCvor->sljedeci;
        delete trenutniCvor;
        trenutniCvor = pocetniCvor;
        velicina--;
        return;
    }
    Cvor* pomocni = pocetniCvor;
    while(pomocni->sljedeci != trenutniCvor)
        pomocni = pomocni->sljedeci;
    pomocni->sljedeci = trenutniCvor->sljedeci;
    delete trenutniCvor;
    if(pomocni->sljedeci == nullptr)
        trenutniCvor = pomocni;
    else trenutniCvor = pomocni->sljedeci;
    velicina--;
}

template <typename T>
T& JednostrukaLista<T>::operator[](int index) const {
    if(index < 0 || index >= velicina) throw "Neispravan indeks";
    Cvor* pomocni = pocetniCvor;
    for(int i = 0; i < index; i++)
        pomocni = pomocni->sljedeci;
    return pomocni->element;
}

template <typename T>
void JednostrukaLista<T>::ispisi() {
    Cvor* pomocni = pocetniCvor;
    while(pomocni != nullptr) {
        std::cout << pomocni->element << " ";
        pomocni = pomocni->sljedeci;
    }
    std::cout << "\n";
}

template <typename T>
void JednostrukaLista<T>::dodajIspred(const T& el) {
    Cvor* novi = new Cvor{nullptr, el};
    if(velicina == 0) {
        trenutniCvor = novi;
        pocetniCvor = novi;
        velicina++;
        return;
    }
    if(pocetniCvor == trenutniCvor) {
        pocetniCvor = novi;
        pocetniCvor->sljedeci = trenutniCvor;
        velicina++;
        return;
    }
    Cvor* pomocni = pocetniCvor;
    while(pomocni->sljedeci != trenutniCvor)
        pomocni = pomocni->sljedeci;
    pomocni->sljedeci = novi;
    novi->sljedeci = trenutniCvor;
    velicina++;
}

template <typename T>
void JednostrukaLista<T>::dodajIza(const T& el) {
    Cvor* novi = new Cvor{nullptr, el};
    if(velicina == 0) {
        trenutniCvor = novi;
        pocetniCvor = novi;
        velicina++;
        return;
    }
    novi->sljedeci = trenutniCvor->sljedeci;
    trenutniCvor->sljedeci = novi;
    velicina++;
}

template <typename T>
class Stek {
    JednostrukaLista<T> lista;
public:
    Stek() {}
    Stek(const Stek& s): lista(s.lista) {}
    Stek& operator=(const Stek& s) {
        if(this == &s) return *this;
        lista = s.lista;
        return *this;
    }
    void stavi(const T& element) {
        lista.dodajIspred(element);
        lista.prethodni();
    }
    void brisi() {
        while(lista.brojElemenata() > 0)
            lista.obrisi();
    }
    T& vrh() {
        if(lista.brojElemenata() == 0) throw "Stek je prazan!";
        return lista.trenutni();
    }
    T skini() {
        if(lista.brojElemenata() == 0) throw "Stek je prazan!";
        lista.pocetak();
        T vratiti = lista.trenutni();
        lista.obrisi();
        return vratiti;
    }
    bool prazan() {
        return lista.brojElemenata() == 0;
    }   
    void ispisi() {
        lista.ispisi();
    }
    int brojElemenata() {
        return lista.brojElemenata();
    }
};

int binarnaPretraga(std::vector<int>& v, int trazeni) {
    int l = 0, d = v.size()-1;
    while(l <= d) {
        int s = l + (d-l)/2;
        if(v[s] == trazeni) return s;
        if(v[s] < trazeni) l = s+1;
        else d = s-1;
    }
    return -1;
}

void pretraga(Stek<std::vector<int>>& s, int trazeni) {
    if(s.brojElemenata() == 0) {
        std::cout << "Nema elementa";
        return;
    }
    std::vector<int> vrh = s.skini();
    int index = binarnaPretraga(vrh, trazeni);
    if(index == -1) 
        pretraga(s, trazeni);
    else std::cout << index << " " << s.brojElemenata();
    
    s.stavi(vrh);
}
int main() {
    Stek<std::vector<int>> s;
    s.stavi(std::vector<int>({9,10,130}));
    s.stavi(std::vector<int>({1,2,3,4,5}));
    s.stavi(std::vector<int>({6,8}));
    s.stavi(std::vector<int>({157,226,3424}));
    pretraga(s, 10);
    return 0;
}
