#include<iostream>

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
    void stavi(const T& element) {
        prvi = new Cvor{ element, prvi };
        velicina++;
    }
    void brisi() {
        while(!this->prazan())
            skini();
    }
    T vrh() {
        return prvi->element;
    }
    T skini() {
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
};


int main() {
Stek<int> s;
for (int i(1); i<=5; i++)
	s.stavi(i);
std::cout << s.brojElemenata() << " " << s.vrh() << " ";
s.brisi();
for (int i(1); i<=5; i++)
	s.stavi(i);
std::cout << s.brojElemenata() << " ";
for (int i(1); i<=5; i++)
	std::cout << s.skini() << " ";
std::cout << s.brojElemenata() << " ";
for (int i(1); i<=5; i++)
	s.stavi(i);
std::cout << s.brojElemenata() << " " << s.vrh();
}
