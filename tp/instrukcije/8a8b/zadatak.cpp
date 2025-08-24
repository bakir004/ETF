#include<iostream>

template <typename TipElemenata>
struct Cvor {
  TipElemenata element;
  Cvor* veza;
};

template <typename TipElemenata>
Cvor<TipElemenata> *KreirajPovezanuListu(const TipElemenata &zavrsni) {
  Cvor<TipElemenata>* pocetak = nullptr;
  Cvor<TipElemenata>* pomocni = nullptr;
  while(true) {
    TipElemenata unos;
    std::cin >> unos;
    if(unos == zavrsni) break;

    if(pocetak == nullptr) {
      pocetak = new Cvor<TipElemenata>{unos, nullptr};
      pomocni = pocetak;
    } else {
      Cvor<TipElemenata>* novi = new Cvor<TipElemenata>{unos, nullptr};
      pomocni->veza = novi;
      pomocni = pomocni->veza;
    }
  }

  return pocetak;
}

template <typename TipElemenata>
int BrojElemenata(Cvor<TipElemenata> *pocetak) {
  // int brojElemenata = 0;
  // for(Cvor<TipElemenata>* pomocni = pocetak; pomocni != nullptr; pomocni = pomocni->veza) {
  //     brojElemenata++;
  // }
  //
  int brojElemenata = 0;
  Cvor<TipElemenata>* pomocni = pocetak;
  while(pomocni != nullptr) {
    brojElemenata++;
    pomocni= pomocni->veza;
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

  // Cvor<TipElemenata>* pomocni = pocetak;
  //
  // while(pomocni != nullptr) {
  //   pomocni = pomocni->veza;
  //   delete pocetak;
  //   pocetak = pomocni;
  // }
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
