#include <iostream>

const int NIZ_LISTA_POCETNI_KAPACITET = 16;
const float MULTIPLIKATOR_KAPACITETA = 1.5;

template <typename T>
class Lista {
public:
    int brojElemenata() const;
    T& trenutni() const;
    bool sljedeci();
    bool prethodni();
    void pocetak();
    void kraj();
    void obrisi();
    void dodajIspred(const T& el);
    void dodajIza(const T& el);
    T& operator[](int index);
};

template <typename T>
class NizLista : public Lista<T> {
    int kapacitet, velicina;
    T* elementi;
    int indexTrenutnog;
    void pomjeriZaJedan(int); 
    void pomjeriZaJedanUnazad(int); 
    void realociraj();
    void testPraznine() { if(velicina == 0) throw "Prazan"; };
public:
    NizLista(): kapacitet(NIZ_LISTA_POCETNI_KAPACITET), velicina(0), elementi(new int[kapacitet]), indexTrenutnog(-1) {}
    void dodajIspred(const T& el);
    void dodajIza(const T& el);
    void ispisi() const;
    int brojElemenata() const { return velicina; }
    T& trenutni() { testPraznine(); return elementi[indexTrenutnog]; }
    void pocetak() { testPraznine(); indexTrenutnog = 0; }
    void kraj() { testPraznine(); indexTrenutnog = velicina-1; }
    bool sljedeci();
    bool prethodni();
    void obrisi() {
        testPraznine();
        if(indexTrenutnog != velicina-1)
            pomjeriZaJedanUnazad(indexTrenutnog);
        velicina--;
    }
    T& operator[](int x) { testPraznine(); return elementi[indexTrenutnog]; }
    ~NizLista() { delete[] elementi; }
};
template <typename T>
void NizLista<T>::realociraj() {
    kapacitet *= MULTIPLIKATOR_KAPACITETA;
    T* noviElementi = new T[kapacitet];
    for(int i = 0; i < velicina; i++)
        noviElementi[i] = elementi[i];
    delete[] elementi;
    elementi = noviElementi;
    std::cout << "REALOCIRAO\n";
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
        std::cout << elementi[i] << " ";
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
void NizLista<T>::dodajIspred(const T& el) {
    if(velicina == 0) {
        elementi[0] = el;
        indexTrenutnog = 0;
        velicina++;
        return;
    }
    if(velicina == kapacitet)
        realociraj();
    velicina++;
    this->pomjeriZaJedan(indexTrenutnog);
    elementi[indexTrenutnog] = el;
    indexTrenutnog++;
}

template <typename T>
void NizLista<T>::dodajIza(const T& el) {
    if(velicina == 0) {
        elementi[0] = el;
        indexTrenutnog = 0;
        velicina++;
        return;
    }
    if(velicina == kapacitet)
        realociraj();
    velicina++;
    this->pomjeriZaJedan(indexTrenutnog+1);
    elementi[indexTrenutnog+1] = el;
}

template <typename T>
class JednostrukaLista : public Lista<T> {
    struct Cvor {
        Cvor* sljedeci;
        T element;
    };
    int velicina;
    Cvor *trenutniCvor, *pocetniCvor;
    void testPraznine() { if(velicina == 0) throw "Prazan"; };
public:
    JednostrukaLista(): velicina(0), trenutniCvor(nullptr), pocetniCvor(nullptr) {}
    int brojElemenata() const { return velicina; }
    T& trenutni() const { testPraznine(); return trenutniCvor->element; }
    bool sljedeci();
    bool prethodni();
    void pocetak();
    void kraj();
    void obrisi();
    void dodajIspred(const T& el);
    void dodajIza(const T& el);
    T& operator[](int index);
    void ispisi();
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
    Cvor* pomocni = pocetniCvor;
    while(pomocni->sljedeci != trenutniCvor)
        pomocni = pomocni->sljedeci;
    pomocni->sljedeci = trenutniCvor->sljedeci;
    delete trenutniCvor;
    velicina--;
}

template <typename T>
T& JednostrukaLista<T>::operator[](int index) {
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
        novi->sljedeci = trenutniCvor;
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
    lista.dodajIspred(5);
    lista.dodajIspred(6);
    lista.dodajIspred(7);
    lista.dodajIza(8);
    lista.dodajIza(9);
    lista.dodajIza(10);
    lista.ispisi();
    lista.prethodni();
    lista.obrisi();
    lista.ispisi();

    return 0;
}
