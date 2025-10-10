#include <string>
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
            std::cout << pomocni->element << " ";
            pomocni = pomocni->sljedeci;
        }
        std::cout << std::endl;
    }
};

bool PravilneZagrade(std::string s) {
    Stek<char> stek;
    for(int i = 0; i < s.size(); i++) {
        if(s[i] == '(' || s[i] == '{' || s[i] == '[') {
            stek.stavi(s[i]);
        } else if(s[i] == ')') {
            if(stek.prazan() || stek.vrh() != '(') return false;
            stek.skini();
        } else if(s[i] == '}') {
            if(stek.prazan() || stek.vrh() != '{') return false;
            stek.skini();
        } else if(s[i] == ']') {
            if(stek.prazan() || stek.vrh() != '[') return false;
            stek.skini();
        }
    }
    return stek.prazan();
}

int main() {
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    s.ispis();
    Stek<int> s2(s);
    s2.ispis();
    s2.skini();
    s2.ispis();
    s.ispis();
    s2.skini();
    s2.skini();
    try {
        s2.skini();
    } catch(const char* e) {
        cout << e << endl;
    }
    s2.ispis();
    s2 = s;
    s2.ispis();
    s2.brisi();
    s2.ispis();
    // primjena steka
    std::string str1 = "(({}{}[][]()))";
    std::cout << std::boolalpha << PravilneZagrade(str1) << std::endl;
    std::string str2 = "(({}{}[][]())";
    std::cout << std::boolalpha << PravilneZagrade(str2) << std::endl;
    return 0;
}
