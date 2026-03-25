#include<iostream>

const int NIZ_LISTA_POCETNI_KAPACITET = 16;
const float MULTIPLIKATOR_KAPACITETA = 1.5;

template <typename T>
class ListaIterator;

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
    virtual void ispisi() const = 0;
    virtual ~Lista() {}
};

template <typename T>
class DvostrukaLista : public Lista<T> {
    friend class ListaIterator<T>;
    struct Cvor {
        Cvor* prethodni;
        Cvor* sljedeci;
        T element;
    };
    int velicina;
    Cvor *trenutniCvor, *pocetniCvor, *posljednjiCvor;
    void testPraznine() const { if(velicina == 0) throw "Prazan"; };
public:
    DvostrukaLista(): velicina(0), trenutniCvor(nullptr), pocetniCvor(nullptr), posljednjiCvor(nullptr) { }
    DvostrukaLista(const DvostrukaLista& lista): velicina(0) {
        Cvor* pomocni = lista.pocetniCvor;
        while(pomocni != nullptr) {
            this->dodajIza(pomocni->element);
            pomocni = pomocni->sljedeci;
            sljedeci();
        }
    }
    DvostrukaLista& operator=(const DvostrukaLista& lista) {
        if(this == &lista) return *this;
        Cvor* pomocni = pocetniCvor;
        while(pomocni != nullptr) {
            Cvor* trenutni = pomocni;
            pomocni = pomocni->sljedeci;
            delete trenutni;
        }
        velicina = 0;
        pocetniCvor = nullptr;
        trenutniCvor = nullptr;
        posljednjiCvor = nullptr;
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
    void brisi();
    void dodajIspred(const T& el) override;
    void dodajIza(const T& el) override;
    T& operator[](int index) const override;
    void ispisi() const override;
    ~DvostrukaLista() { brisi(); }
};

template <typename T>
void DvostrukaLista<T>::brisi() {
    Cvor* pomocni = pocetniCvor;
    while(pomocni != nullptr) {
        Cvor* trenutni = pomocni;
        pomocni = pomocni->sljedeci;
        delete trenutni;
    }
    posljednjiCvor = nullptr;
    pocetniCvor = nullptr;
    trenutniCvor = nullptr;
    velicina = 0;
}

template <typename T>
void DvostrukaLista<T>::ispisi() const {
    Cvor* pomocni = pocetniCvor;
    while(pomocni != nullptr) {
        std::cout << pomocni->element << " ";
        pomocni = pomocni->sljedeci;
    }
    std::cout << "\n";
}

template <typename T>
T& DvostrukaLista<T>::operator[](int index) const {
    if(index < 0 || index >= velicina) throw "Neispravan indeks";
    if(brojElemenata() - index > index) {
        Cvor* pomocni = pocetniCvor;
        for(int i = 0; i < index; i++)
            pomocni = pomocni->sljedeci;
        return pomocni->element;
    } else {
        Cvor* pomocni = posljednjiCvor;
        for(int i = velicina-1; i > index; i--)
            pomocni = pomocni->prethodni;
        return pomocni->element;
    }
}

template <typename T>
void DvostrukaLista<T>::dodajIza(const T& el) {
    Cvor* novi = new Cvor{nullptr, nullptr, el};
    if(velicina == 0) {
        trenutniCvor = novi;
        pocetniCvor = novi;
        posljednjiCvor = novi;
        velicina++;
        return;
    }
    if(posljednjiCvor == trenutniCvor) {
        posljednjiCvor->sljedeci = novi;
        novi->prethodni = posljednjiCvor;
        posljednjiCvor = novi;
        velicina++;
        return;
    }
    novi->sljedeci = trenutniCvor->sljedeci;
    novi->prethodni = trenutniCvor;
    trenutniCvor->sljedeci->prethodni = novi;
    trenutniCvor->sljedeci = novi;
    velicina++;
}

template <typename T>
void DvostrukaLista<T>::dodajIspred(const T& el) {
    Cvor* novi = new Cvor{nullptr, nullptr, el};
    if(velicina == 0) {
        trenutniCvor = novi;
        pocetniCvor = novi;
        posljednjiCvor = novi;
        velicina++;
        return;
    }
    if(pocetniCvor == trenutniCvor) {
        novi->sljedeci = pocetniCvor;
        pocetniCvor->prethodni = novi;
        pocetniCvor = novi;
        velicina++;
        return;
    }
    novi->sljedeci = trenutniCvor;
    novi->prethodni = trenutniCvor->prethodni;
    trenutniCvor->prethodni->sljedeci = novi;
    trenutniCvor->prethodni = novi;
    velicina++;
}

template <typename T>
void DvostrukaLista<T>::obrisi() {
    testPraznine();
    if(pocetniCvor == trenutniCvor) {
        pocetniCvor = pocetniCvor->sljedeci;
        delete trenutniCvor;
        trenutniCvor = pocetniCvor;
        if(trenutniCvor != nullptr)
            trenutniCvor->prethodni = nullptr;
        velicina--;
        return;
    }
    if(posljednjiCvor == trenutniCvor) {
        posljednjiCvor = posljednjiCvor->prethodni;
        delete trenutniCvor;
        trenutniCvor = posljednjiCvor;
        if(trenutniCvor != nullptr)
           trenutniCvor->sljedeci = nullptr;
        velicina--;
        return;
    }
    trenutniCvor->prethodni->sljedeci = trenutniCvor->sljedeci;
    trenutniCvor->sljedeci->prethodni = trenutniCvor->prethodni;
    Cvor* pomocni = trenutniCvor->sljedeci;
    delete trenutniCvor;
    trenutniCvor = pomocni;
    velicina--;
}

template <typename T>
void DvostrukaLista<T>::kraj() {
    testPraznine();
    trenutniCvor = posljednjiCvor;
}

template <typename T>
void DvostrukaLista<T>::pocetak() {
    testPraznine();
    trenutniCvor = pocetniCvor;
}

template <typename T>
bool DvostrukaLista<T>::prethodni() {
    testPraznine();
    if(trenutniCvor == pocetniCvor) return false;
    trenutniCvor = trenutniCvor->prethodni;
    return true;
}

template <typename T>
bool DvostrukaLista<T>::sljedeci() {
    testPraznine();
    if(trenutniCvor->sljedeci == nullptr) return false;
    trenutniCvor = trenutniCvor->sljedeci;
    return true;
}

template <typename T>
class DvostraniRed {
    DvostrukaLista<T> lista;
public:
    DvostraniRed() {}
    DvostraniRed(const DvostraniRed& red): lista(red.lista) {}
    DvostraniRed& operator=(const DvostraniRed& red) {
        if(this == &red) return *this;
        lista = red.lista;
        return *this;
    }
    void staviNaVrh(const T& el) { 
        if(lista.brojElemenata() != 0) lista.kraj();
        lista.dodajIza(el); 
    }
    void staviNaCelo(const T& el) { 
        if(lista.brojElemenata() != 0) lista.pocetak();
        lista.dodajIspred(el); 
    }
    T skiniSaVrha() {
        if(lista.brojElemenata() == 0) throw "Prazan red";
        lista.kraj();
        int el = lista.trenutni();
        lista.obrisi();
        return el;
    }
    T skiniSaCela() {
        if(lista.brojElemenata() == 0) throw "Prazan red";
        lista.pocetak();
        int el = lista.trenutni();
        lista.obrisi();
        return el;
    }
    T vrh() {
        if(lista.brojElemenata() == 0) throw "Prazan red";
        lista.kraj();
        return lista.trenutni();
    }
    T celo() {
        if(lista.brojElemenata() == 0) throw "Prazan red";
        lista.pocetak();
        return lista.trenutni();
    }
    int brojElemenata() const { return lista.brojElemenata(); }
    void brisi() { lista.brisi(); }
    void ispisi() { lista.ispisi(); }
};

int main() {
    DvostraniRed<int> red;
    red.staviNaVrh(1);
    red.staviNaVrh(2);
    red.staviNaVrh(3);
    red.staviNaCelo(4);
    red.staviNaCelo(5);
    red.staviNaCelo(6);
    red.ispisi();
    red.skiniSaVrha();
    red.skiniSaCela();
    red.ispisi();
    std::cout << red.brojElemenata() << std::endl;
    std::cout << red.vrh() << "*" << red.celo() << "=" << red.vrh()*red.celo() << std::endl;
    red.brisi();
    std::cout << red.brojElemenata() << std::endl;


    return 0;
}
