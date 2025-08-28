#include <iostream>

class Ptica {
public:
  virtual ~Ptica() = default;
};

class PticaKojaLeti : public Ptica {
public:
  virtual void Leti() { std::cout << "Leti\n"; }
};
class PticaKojaNeLeti : public Ptica {};

class Noj : public PticaKojaNeLeti {};
class Pingvin : public PticaKojaNeLeti {};

class Orao : public PticaKojaLeti {};
class Vrana : public PticaKojaLeti {};

int main() {
  Ptica* p = new Orao;
  
  if(auto k = dynamic_cast<PticaKojaLeti*>(p))
    k->Leti();

  delete p;
  return 0;
}
