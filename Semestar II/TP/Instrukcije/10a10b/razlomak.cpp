#include <iostream>
#include <stdexcept>

class Razlomak {
 int brojnik, nazivnik;
 int nzd(int a, int b) const {
   while (b != 0) {
     int r = a % b;
     a = b;
     b = r;
   }
   return a;
 }
 int nzs(int a, int b) const { return (a / nzd(a, b)) * b; }
 void skrati() {
   int d = nzd(brojnik, nazivnik);
   brojnik /= d;
   nazivnik /= d;
   if (nazivnik < 0) {
     brojnik = -brojnik;
     nazivnik = -nazivnik;
   }
 }
public:
 Razlomak(int b, int n) : brojnik(b), nazivnik(n) {
   if (nazivnik == 0) throw std::domain_error("Nazivnik ne moze biti nula");
   if(nazivnik < 0) {
     brojnik = -brojnik;
     nazivnik = -nazivnik;
   }
 }
 Razlomak& Postavi(int b, int n) {
   if (n == 0) throw std::domain_error("Nazivnik ne moze biti nula");
   brojnik = b;
   nazivnik = n;
   if(nazivnik < 0) {
     brojnik = -brojnik;
     nazivnik = -nazivnik;
   }
   return *this;
 }
 int dajBrojnik() const { return brojnik; }
 int dajNazivnik() const { return nazivnik; }
 void Ispisi() const {
   if(nazivnik == 1) std::cout << brojnik;
   else std::cout << brojnik << "/" << nazivnik;
 }
 Razlomak& SaberiSa(const Razlomak& r) {
   int noviNazivnik = nzs(nazivnik, r.nazivnik);
   brojnik = brojnik * noviNazivnik / nazivnik + r.brojnik * noviNazivnik / r.nazivnik;
   nazivnik = noviNazivnik;
   skrati();
   return *this;
 }
 Razlomak& OduzmiMu(const Razlomak& r) {
   int noviNazivnik = nzs(nazivnik, r.nazivnik);
   brojnik = brojnik * noviNazivnik / nazivnik - r.brojnik * noviNazivnik / r.nazivnik;
   nazivnik = noviNazivnik;
   skrati();
   return *this;
 }
 Razlomak& PomnoziSa(const Razlomak& r) {
   brojnik *= r.brojnik;
   nazivnik *= r.nazivnik;
   skrati();
   return *this;
 }
 Razlomak& PodijeliSa(const Razlomak& r) {
   if (r.brojnik == 0) throw std::domain_error("Dijeljenje s nulom");
   brojnik *= r.nazivnik;
   nazivnik *= r.brojnik;
   skrati();
   return *this;
 }
 Razlomak& Reciprociraj() {
   if (brojnik == 0) throw std::domain_error("Reciprocna vrijednost nula");
   std::swap(brojnik, nazivnik);
   skrati();
   return *this;
 }
 friend Razlomak SaberiRazlomke(const Razlomak& r1, const Razlomak& r2);
 friend Razlomak OduzmiRazlomke(const Razlomak& r1, const Razlomak& r2);
 friend Razlomak PomnoziRazlomke(const Razlomak& r1, const Razlomak& r2);
 friend Razlomak PodijeliRazlomke(const Razlomak& r1, const Razlomak& r2);
};

Razlomak SaberiRazlomke(const Razlomak& r1, const Razlomak& r2) {
 Razlomak rezultat = r1;
 rezultat.SaberiSa(r2);
 return rezultat;
}
Razlomak OduzmiRazlomke(const Razlomak& r1, const Razlomak& r2) {
 Razlomak rezultat = r1;
 rezultat.OduzmiMu(r2);
 return rezultat;
}
Razlomak PomnoziRazlomke(const Razlomak& r1, const Razlomak& r2) {
 Razlomak rezultat = r1;
 rezultat.PomnoziSa(r2);
 return rezultat;
}
Razlomak PodijeliRazlomke(const Razlomak& r1, const Razlomak& r2) {
 Razlomak rezultat = r1;
 rezultat.PodijeliSa(r2);
 return rezultat;
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
