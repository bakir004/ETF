#include <iostream>
using namespace std;
template<typename T>
class Stek {
    struct Cvor {
        T element;
        Cvor* sljedeci;
    };
    Cvor* prvi;
    int velicina;
public:
    Stek() : prvi(nullptr), velicina(0) {}
    Stek(const Stek& s) : prvi(nullptr), velicina(0) {
        Stek<T> pomocni;
        Cvor* pomocniCvor = s.prvi;
        while(pomocniCvor != nullptr) {
            pomocni.stavi(pomocniCvor->element);
            pomocniCvor = pomocniCvor->sljedeci;
        }
        while(!pomocni.prazan()) {
            stavi(pomocni.skini());
        }
    }
    Stek& operator=(const Stek& s) {
        if(this == &s) return *this;
        Stek<T> pomocni;
        Cvor* pomocniCvor = s.prvi;
        while(pomocniCvor != nullptr) {
            pomocni.stavi(pomocniCvor->element);
            pomocniCvor = pomocniCvor->sljedeci;
        }
        while(!pomocni.prazan()) {
            stavi(pomocni.skini());
        }
        return *this;
    }
    void stavi(const T& element) {
        prvi = new Cvor{ element, prvi };
        velicina++;
    }
    void brisi() {
        while(!this->prazan())
            skini();
    }
    T& vrh() {
        return prvi->element;
    }
    T skini() {
        if(velicina == 0) throw "Stek je prazan!";
        T vratiti = vrh();
        Cvor* prosli = prvi;
        prvi = prvi->sljedeci;
        delete prosli;
        velicina--;
        return vratiti;
    }
    bool prazan() {
        return velicina == 0;
    }
    int brojElemenata() {
        return velicina;
    }
    ~Stek() {
        while(prvi != nullptr) {
            Cvor* prosli = prvi;
            prvi = prvi->sljedeci;
            delete prosli;
        }
    }
    void ispis() {
        Cvor* pomocni = prvi;
        while(pomocni != nullptr) {
            cout << pomocni->element << " ";
            pomocni = pomocni->sljedeci;
        }
        cout << endl;
    }
};
