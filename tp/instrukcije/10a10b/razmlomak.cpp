#include <iostream>
#include <stdexcept>

class Razlomak {
  int brojnik, nazivnik;
  int NZD(int a, int b) {
    if(b == 0) return a;
    return NZD(b, a % b);
  }
  int NZS(int a, int b) {
    return a * b / NZD(a,b);
  }
public:
  Razlomak(int b = 0, int n = 1) {
    Postavi(b,n);
  }
  Razlomak& Postavi(int b, int n) {
    if(n == 0) throw std::domain_error("Nazivnik ne moze biti 0");
    if(n < 0) {
      b = -b;
      n = -n;
    }
    int nzd = std::abs(NZD(b, n));
    brojnik = b / nzd;
    nazivnik = n / nzd;
    return *this; // this je pointer na instancu klase pa moramo dereferencirati
  }
  int DajBrojnik() const { return brojnik; }
  int DajNazivnik() const { return nazivnik; }
  Razlomak& Ispisi() {
    if(nazivnik == 1 || brojnik == 0) std::cout << brojnik;
    else std::cout << brojnik << "/" << nazivnik;
    return *this;
  }
  Razlomak& SaberiSa(const Razlomak& r) {
    int nzs = NZS(nazivnik, r.nazivnik);
    int b = nzs / nazivnik * brojnik + nzs / r.nazivnik * r.brojnik;
    int n = nzs;
    Postavi(b, n);
    return *this;
  }
  Razlomak& OduzmiMu(const Razlomak& r) {
    int nzs = NZS(nazivnik, r.nazivnik);
    int b = nzs / nazivnik * brojnik - nzs / r.nazivnik * r.brojnik;
    int n = nzs;
    Postavi(b, n);
    return *this;
  }
  Razlomak& PomnoziSa(const Razlomak& r) {
    int b = brojnik * r.brojnik;
    int n = nazivnik * r.nazivnik;
    Postavi(b, n);
    return *this;
  }
  Razlomak& PodijeliSa(const Razlomak& r) {
    int b = brojnik * r.nazivnik;
    int n = nazivnik * r.brojnik;
    Postavi(b, n);
    return *this;
  }
  Razlomak& Reciprociraj() {
    if(brojnik == 0) throw std::domain_error("Ne postoji reciprocna vrijednost nule");
    int temp = brojnik;
    brojnik = nazivnik;
    nazivnik = temp;

    if(nazivnik < 0) {
      brojnik = -brojnik;
      nazivnik = -nazivnik;
    }
    return *this;
  }
  friend Razlomak SaberiRazlomke(const Razlomak& r1, const Razlomak& r2);
  friend Razlomak OduzmiRazlomke(const Razlomak& r1, const Razlomak& r2);
  friend Razlomak PomnoziRazlomke(const Razlomak& r1, const Razlomak& r2);
  friend Razlomak PodijeliRazlomke(const Razlomak& r1, const Razlomak& r2);
};

Razlomak SaberiRazlomke(const Razlomak& r1, const Razlomak& r2) {
  Razlomak r(r1.brojnik, r1.nazivnik);
  r.SaberiSa(r2);
  return r;
}
Razlomak OduzmiRazlomke(const Razlomak& r1, const Razlomak& r2) {
  Razlomak r(r1.brojnik, r1.nazivnik);
  r.OduzmiMu(r2);
  return r;
}
Razlomak PomnoziRazlomke(const Razlomak& r1, const Razlomak& r2) {
  Razlomak r(r1.brojnik, r1.nazivnik);
  r.PomnoziSa(r2);
  return r;
}
Razlomak PodijeliRazlomke(const Razlomak& r1, const Razlomak& r2) {
  Razlomak r(r1.brojnik, r1.nazivnik);
  r.PodijeliSa(r2);
  return r;
}

int main() {
  Razlomak r1(-3, 4);
  Razlomak r2(5, -6);
  r1.Ispisi(); std::cout << " + "; r2.Ispisi(); std::cout << " = ";
  Razlomak r3 = SaberiRazlomke(r1, r2);
  r3.Ispisi(); std::cout << std::endl;
  r1.Ispisi(); std::cout << " - "; r2.Ispisi(); std::cout << " = ";
  Razlomak r4 = OduzmiRazlomke(r1, r2);
  r4.Ispisi(); std::cout << std::endl;
  r1.Ispisi(); std::cout << " * "; r2.Ispisi(); std::cout << " = ";
  Razlomak r5 = PomnoziRazlomke(r1, r2);
  r5.Ispisi(); std::cout << std::endl;
  r1.Ispisi(); std::cout << " / "; r2.Ispisi(); std::cout << " = ";
  Razlomak r6 = PodijeliRazlomke(r1, r2);
  r6.Ispisi(); std::cout << std::endl;
}
