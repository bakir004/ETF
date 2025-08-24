#include<iostream>
#include<list>
#include<complex>

struct Kompleksni {
  double re, im;
  Kompleksni* sljedeci;
};

Kompleksni* KreirajListu(std::list<std::complex<double>> lista) {
  Kompleksni* pocetak = nullptr;
  Kompleksni* pomocni = nullptr;

  for(auto kompleksniBroj : lista) {
    if(pocetak == nullptr) { // pocetak jos nije definisan, dakle trebam napravit pocetak
      pocetak = new Kompleksni{kompleksniBroj.real(), kompleksniBroj.imag(), nullptr};
      pomocni = pocetak;
    } else { // pocetak postoji (nije nullptr) dakle alociram novi cvor i vezem za stari cvor
      Kompleksni* novi = new Kompleksni{kompleksniBroj.real(), kompleksniBroj.imag(), nullptr}; 
      pomocni->sljedeci = novi;
      pomocni = pomocni->sljedeci;
    }
  }

  return pocetak;
}

void BrisiListu(Kompleksni* pocetak) {
  Kompleksni* pomocni = pocetak;
  while(pomocni != nullptr) {
    pomocni = pocetak->sljedeci;
    delete pocetak;
    pocetak = pomocni;
  }
}

int main() {
  std::list<std::complex<double>> lista{ {1,2}, {3,4}, {5,6} };
  Kompleksni* pocetak = KreirajListu(lista);
  auto pomocni = pocetak;
  while(pomocni != nullptr) {
    std::cout << pomocni->re << "," << pomocni->im << "\n";
    pomocni = pomocni->sljedeci;
  }
  BrisiListu(pocetak);

  return 0;
}
