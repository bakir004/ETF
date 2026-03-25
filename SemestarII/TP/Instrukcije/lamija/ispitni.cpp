#include<iostream>
#include<list>
#include<complex>

struct Kompleksni {
    double re, im;
    Kompleksni* sljedeci;
};

Kompleksni* KreirajListu(std::list<std::complex<double>>
                         listaKompleksnih) {
    Kompleksni* pocetak = nullptr;
    Kompleksni* kraj = nullptr;
    for(auto element : listaKompleksnih) {
        Kompleksni* novi = 
            new Kompleksni{element.real(), element.imag(),
                nullptr
            };

        if(pocetak == nullptr) {
            pocetak = novi;
            kraj = novi;
        } else {
            kraj->sljedeci = novi;
            kraj = novi;
        }
    }

    return pocetak;
}

int main() {
    std::list<std::complex<double>> lista{{1,2},{3,4},{5,6}};

    auto pocetak = KreirajListu(lista);

    for(auto p = pocetak; p != nullptr; p = p->sljedeci) {
        std::cout << "Kompleksni " << p->re << ", " << p->im << "\n";
    }
    return 0;
}
