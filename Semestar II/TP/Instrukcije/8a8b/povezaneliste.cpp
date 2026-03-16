#include<iostream>

template <typename Tip>
struct Cvor {
  Tip vrijednost;
  Cvor* sljedeci;
};

int main() {

  Cvor<int>* pocetak = new Cvor<int>{1,nullptr};
  Cvor<int>* pomocni = pocetak;

  // KREIRANJE 
  for(int i = 2; i < 7; i++) { // granice petlje cisto da elementi liste imaju neke vrijednosti
    Cvor<int>* novi = new Cvor<int>{i, nullptr};
    pomocni->sljedeci = novi;
    pomocni = pomocni->sljedeci;
  }

  // PROLAZAK KROZ LISTU 
  pomocni = pocetak;
  int velicina = 0;
  while(pomocni != nullptr) {
    std::cout << pomocni->vrijednost << " ";
    velicina++;
    pomocni = pomocni->sljedeci;
  }

  std::cout << "\nUNAZAD:\n";
  // PROLAZAK KROZ LISTU UNAZAD
  int brojPomjeranjaZasad = velicina-1;
  for(int i = 0; i < velicina; i++) {    
    pomocni = pocetak;
    for(int i = 0; i < brojPomjeranjaZasad; i++) {
      pomocni = pomocni->sljedeci;
    }
    std::cout << pomocni->vrijednost << " ";
    brojPomjeranjaZasad--;
    if(brojPomjeranjaZasad < 0) break;
  }

  // BRISANJE
  pomocni = pocetak;
  while(pomocni != nullptr) {
    pomocni = pocetak->sljedeci;
    delete pocetak;
    pocetak = pomocni;
  }

  return 0;
}
