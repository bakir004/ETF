#include <iostream>

const int NIZ_LISTA_POCETNI_KAPACITET = 16;
const float MULTIPLIKATOR_KAPACITETA = 1.5;

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
class NizLista : public Lista<T> {
    int kapacitet, velicina;
    T** elementi;
    int indexTrenutnog;
    void pomjeriZaJedan(int); 
    void pomjeriZaJedanUnazad(int); 
    void realociraj();
    void testPraznine() const { if(velicina == 0) throw "Prazan"; };
public:
    NizLista(): kapacitet(NIZ_LISTA_POCETNI_KAPACITET), velicina(0), elementi(new T*[kapacitet]{}), indexTrenutnog(-1) {}
    NizLista(const NizLista& niz): velicina(niz.velicina), kapacitet(niz.kapacitet) {
        try {
            elementi = new T*[kapacitet]{};
            for(int i = 0; i < velicina; i++)
                elementi[i] = new T(*niz.elementi[i]);
            indexTrenutnog = niz.indexTrenutnog;
        } catch(...) {
            for(int i = 0; i < velicina; i++)
                delete elementi[i];
            delete[] elementi;
        }
    }
    NizLista& operator=(const NizLista& niz) {
        T** noviElementi = nullptr;
        try {
            noviElementi = new T*[kapacitet]{};
            for(int i = 0; i < velicina; i++)
                noviElementi[i] = new T(*niz.elementi[i]);
        } catch(...) {
            for(int i = 0; i < velicina; i++)
                delete noviElementi[i];
            delete[] noviElementi;
            throw;
        }
        for(int i = 0; i < velicina; i++)
            delete elementi[i];
        delete[] elementi;
        elementi = noviElementi;
        velicina = niz.velicina;
        kapacitet = niz.kapacitet;
        indexTrenutnog = niz.indexTrenutnog;
        return *this;
    }
    void dodajIspred(const T& el) override;
    void dodajIza(const T& el) override;
    void ispisi() const;
    int brojElemenata() const override { return velicina; }
    T& trenutni() const override { testPraznine(); return *elementi[indexTrenutnog]; }
    void pocetak() override { testPraznine(); indexTrenutnog = 0; }
    void kraj() override { testPraznine(); indexTrenutnog = velicina-1; }
    bool sljedeci() override;
    bool prethodni() override;
    void obrisi() override;
    T& operator[](int index) const override { 
        testPraznine();   
        if(index < 0 || index >= velicina) throw "Neispravan indeks";
        return *elementi[index]; 
    }
    ~NizLista() { 
        for(int i = 0; i < velicina; i++)
            delete elementi[i];
        delete[] elementi;
        elementi = nullptr;
    }
};
template <typename T>
void NizLista<T>::realociraj() {
    kapacitet *= MULTIPLIKATOR_KAPACITETA;
    T** noviElementi = nullptr;
    try {
        noviElementi = new T*[kapacitet]{};
        for(int i = 0; i < velicina; i++)
            noviElementi[i] = new T(*elementi[i]);
        for(int i = 0; i < velicina; i++)
            delete elementi[i];
        delete[] elementi;
    } catch(...) {
        for(int i = 0; i < velicina; i++)
            delete noviElementi[i];
        delete[] noviElementi;
        throw;
    }
    elementi = noviElementi;
    /*std::cout << "REALOCIRAO\n";*/
}

template <typename T>
void NizLista<T>::obrisi() {
    testPraznine();
    delete elementi[indexTrenutnog];
    if(indexTrenutnog != velicina-1)
        pomjeriZaJedanUnazad(indexTrenutnog);
    else 
        indexTrenutnog--;
    velicina--;
}

template <typename T>
void NizLista<T>::dodajIspred(const T& el) {
    if(velicina == 0) {
        elementi[0] = new T(el);
        indexTrenutnog = 0;
        velicina++;
        return;
    }
    if(velicina == kapacitet)
        realociraj();
    velicina++;
    this->pomjeriZaJedan(indexTrenutnog);
    elementi[indexTrenutnog] = new T(el);
    indexTrenutnog++;
}

template <typename T>
bool NizLista<T>::prethodni() {
    testPraznine();
    if(indexTrenutnog == 0) return false;
    indexTrenutnog--;
    return true;
}

template <typename T>
bool NizLista<T>::sljedeci() {
    testPraznine();
    if(indexTrenutnog == velicina-1) return false;
    indexTrenutnog++;
    return true;
}

template <typename T>
void NizLista<T>::ispisi() const {
    for(int i = 0; i < velicina; i++)
        std::cout << *elementi[i] << " ";
    std::cout << "\n";
}

template <typename T>
void NizLista<T>::pomjeriZaJedan(int indexPocetkaPomjeranja) {
    for(int i = velicina - 1; i > indexPocetkaPomjeranja; i--)
        elementi[i] = elementi[i-1];
}

template <typename T>
void NizLista<T>::pomjeriZaJedanUnazad(int indexPocetkaPomjeranja) {
    for(int i = indexPocetkaPomjeranja; i < velicina; i++)
        elementi[i] = elementi[i+1];
}


template <typename T>
void NizLista<T>::dodajIza(const T& el) {
    if(velicina == 0) {
        elementi[0] = new T(el);
        indexTrenutnog = 0;
        velicina++;
        return;
    }
    if(velicina == kapacitet)
        realociraj();
    velicina++;
    this->pomjeriZaJedan(indexTrenutnog+1);
    elementi[indexTrenutnog+1] = new T(el);
}








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

int main() {
JednostrukaLista<int> lista;
for (int i(1); i<=10000; i++)
	lista.dodajIspred(i);
std::cout << lista.brojElemenata() << " ";
for (int i(1); i<=10000; i++)
	lista.obrisi();
std::cout << lista.brojElemenata();

    return 0;
}
