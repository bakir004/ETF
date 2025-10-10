#include<iostream>

template <typename TipElemenata>
struct Cvor {
    TipElemenata element;
    Cvor* veza;
};

template <typename TipElemenata>
Cvor<TipElemenata> *KreirajPovezanuListu(const TipElemenata &zavrsni) {
    Cvor<TipElemenata>* pocetak = nullptr;
    Cvor<TipElemenata>* kraj = nullptr;
    while(true) {
        TipElemenata unos;
        std::cin >> unos;
        if(unos == zavrsni) break;

        if(pocetak == nullptr) {
            Cvor<TipElemenata>* novi = new Cvor<TipElemenata>{unos, nullptr};
            pocetak = novi;
            kraj = novi;
        } else {
            Cvor<TipElemenata>* novi = new Cvor<TipElemenata>{unos, nullptr};
            kraj->veza = novi;
            kraj = novi;
        }
    }

    return pocetak;
}

template <typename TipElemenata>
int BrojElemenata(Cvor<TipElemenata> *pocetak) {
    int brojElemenata = 0;
    for(Cvor<TipElemenata>* pomocni = pocetak; pomocni != nullptr; pomocni = pomocni->veza) {
        brojElemenata++;
    }
    return brojElemenata;
}

template <typename TipElemenata>
TipElemenata SumaElemenata(Cvor<TipElemenata> *pocetak) {
    TipElemenata suma = TipElemenata();
    for(Cvor<TipElemenata>* pomocni = pocetak; pomocni != nullptr; pomocni = pomocni->veza) {
        suma += pomocni->element; 
    }
    return suma;
}

template <typename TipElemenata>
int BrojVecihOd(Cvor<TipElemenata> *pocetak, const TipElemenata &prag) {
    int brojVecih = 0;
    for(Cvor<TipElemenata>* pomocni = pocetak; pomocni != nullptr; pomocni = pomocni->veza) {
        if(pomocni->element > prag)
            brojVecih++;
    }
    return brojVecih;
}

template <typename TipElemenata>
void UnistiListu(Cvor<TipElemenata> *pocetak) {
    Cvor<TipElemenata>* pomocni = pocetak->veza;

    while(pomocni != nullptr) {
        delete pocetak;
        pocetak = pomocni;
        pomocni = pomocni->veza;
    }
    delete pocetak;
}

int main() {
    int zavrsni = 0;
    Cvor<int>* pocetak = KreirajPovezanuListu(zavrsni);
    std::cout << BrojElemenata(pocetak) << std::endl;
    std::cout << BrojVecihOd(pocetak, 3) << std::endl;
    std::cout << SumaElemenata(pocetak);
    UnistiListu(pocetak);

    return 0;
}
