#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

const double PI = std::atan(1) * 4;

class Tijelo;

typedef std::shared_ptr<Tijelo> PTijelo;

class Tijelo {
  double gustina;
public:
  Tijelo(double gustina) : gustina(gustina) {}
  double DajGustinu() const { return gustina; }
  virtual double DajZapreminu() const = 0;
  double DajTezinu() const { return DajZapreminu()*DajGustinu(); }
  virtual PTijelo DajKopiju() const = 0;
  virtual void Ispis() const = 0;
};

class Kvadar : public Tijelo {
  double a,b,c;
public:
  Kvadar(double g, double a, double b, double c) : Tijelo(g), a(a), b(b), c(c) {}
  double DajA() const { return a; }
  double DajB() const { return b; }
  double DajC() const { return c; }
  double DajZapreminu() const override { return a*b*c; }
  void Ispis() const override { std::cout << "Kvadar:\n  Tezina: " << DajTezinu() 
    << "\n  Zapremina: " << DajZapreminu() << "\n";}
  PTijelo DajKopiju() const override { return std::make_shared<Kvadar>(*this); }
};

class Lopta : public Tijelo {
  double r;
public:
  Lopta(double g, double r) : Tijelo(g), r(r) {}
  double DajR() const { return r; }
  double DajZapreminu() const override { return 4.*r*r*r*PI/3.; }
  void Ispis() const override { std::cout << "Lopta:\n  Tezina: " << DajTezinu() 
    << "\n  Zapremina: " << DajZapreminu() << "\n";}
  PTijelo DajKopiju() const override { return std::make_shared<Lopta>(*this); }
};
class Valjak : public Tijelo {
  double r,h;
public:
  Valjak(double g, double r, double h) : Tijelo(g), r(r), h(h) {}
  double DajR() const { return r; }
  double DajH() const { return h; }
  double DajZapreminu() const override { return r*r*PI*h; }
  void Ispis() const override { std::cout << "Valjak:\n  Tezina: " << DajTezinu() 
    << "\n  Zapremina: " << DajZapreminu() << "\n";}
  PTijelo DajKopiju() const override { return std::make_shared<Valjak>(*this); }
};


class KolekcijaTijela {
  std::vector<PTijelo> tijela;
public:
  KolekcijaTijela() {}
  void DodajKvadar(double g, double a, double b, double c) {
    tijela.emplace_back(new Kvadar(g,a,b,c));
  }
  void DodajLoptu(double g, double r) {
    tijela.emplace_back(new Lopta(g,r));
  }
  void DodajValjak(double g, double r, double h) {
    tijela.emplace_back(new Valjak(g,r,h));
  }
  int DajVelicinu() const { return tijela.size(); }
  KolekcijaTijela(const KolekcijaTijela& k) {
    for(auto &t : k.tijela)
      tijela.push_back(t->DajKopiju());
  }
  KolekcijaTijela(KolekcijaTijela&& k) = default;
  KolekcijaTijela& operator=(const KolekcijaTijela& k) {
    tijela.clear();
    for(auto &t : k.tijela)
      tijela.push_back(t->DajKopiju());
    return *this;
  }
  KolekcijaTijela& operator=(KolekcijaTijela&& k) = default;

  Tijelo& Najtezi() { 
    return **std::max_element(tijela.begin(), tijela.end(), 
                             [](const PTijelo& t1, const PTijelo& t2){
                                return t1->DajTezinu() < t2->DajTezinu();
                             });
  }
  Tijelo& Najlaksi() { 
    return **std::min_element(tijela.begin(), tijela.end(), 
                             [](const PTijelo& t1, const PTijelo& t2){
                                return t1->DajTezinu() < t2->DajTezinu();
                             });
  }
  Tijelo& operator[](int index) {
    if(index < 0 || index > tijela.size()) throw;
    return *tijela[index];
  }
  int BrojTezihOd(double tezina) const {
    return std::count_if(tijela.begin(), tijela.end(), [&tezina](const PTijelo& p) {
      return p->DajTezinu() > tezina;
    });
  }
  void Sortiraj() {
    std::sort(tijela.begin(), tijela.end(), 
                   [](const PTijelo& t1, const PTijelo& t2){
                      return t1->DajTezinu() < t2->DajTezinu();
                   });
  }
};

int main() {
  // KolekcijaTijela k;
  // k.DodajValjak(2, 4, 8);
  // k.DodajLoptu(2, 8);
  // k.DodajKvadar(2, 4, 8, 10);
  // k.Najtezi().Ispis();
  // k.Najlaksi().Ispis();
  char d;
  double a,b,c,g;
  KolekcijaTijela k;
  while(true) {
    std::cin >> d;
    if(d == '.') break;
    if(d == 'K') {
      std::cin >> g >> a >> b >> c;
      k.DodajKvadar(g, a, b, c);
    } else if(d == 'L') {
      std::cin >> g >> a;
      k.DodajLoptu(g, a);
    } else if(d == 'V') {
      std::cin >> g >> a >> b;
      k.DodajValjak(g, a, b);
    }
  }
  k.Sortiraj();
  for(int i = 0; i < k.DajVelicinu(); i++)
    k[i].Ispis();

  return 0;
}
