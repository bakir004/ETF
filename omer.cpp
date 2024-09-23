#include <iostream>

class Kasa {
    int pare;
public:
    explicit Kasa(int a, int b) {}
    void Postavi(int novac) {
        pare = novac;
    }
    void Ispisi() const {
        std::cout << "Stanje: " << this->pare << "\n";
    }
    Kasa& operator+=(const Kasa& k) {
        this->pare += k.pare;
        return *this;
    }
    Kasa& operator++() {
        this->pare++;
        return *this;
    }   
    Kasa operator++(int) {
        Kasa stara = *this;
        this->pare++;
        return stara;
    }   
    friend Kasa operator+(const Kasa& k1, const Kasa& k2);
    friend std::istream& operator>>(std::istream& tok, Kasa& k);
};

std::istream& operator>>(std::istream& tok, Kasa& k) {
    tok >> k.pare;
    return tok;
}

Kasa operator+(const Kasa& k1, const Kasa& k2) {
    Kasa nova;
    nova.pare = k1.pare + k2.pare;
    return nova;
}

int main() {
    Kasa kasa = Kasa{1,2};
    return 0;

}
















