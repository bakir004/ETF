#include <initializer_list>
#include <iostream>

class MojNizIntova {
private:
  int* brojevi;
  int velicina;
public:
  MojNizIntova(int s) : velicina(s) {
    std::cout << "Konstruktor MojNizIntova(int s)\n";
    brojevi = new int[velicina];
    for (int i = 0; i < velicina; ++i)
      brojevi[i] = 0;
  }

  MojNizIntova(std::initializer_list<int> lista): velicina(lista.size()) {
    std::cout << "Konstruktor MojNizIntova(std::initializer_list<int> lista)\n";
    brojevi = new int[velicina];
    int i = 0;
    for (auto it = lista.begin(); it != lista.end(); ++it, ++i)
      brojevi[i] = *it;
  }

  ~MojNizIntova() {
    std::cout << "Destruktor MojNizIntova\n";
    delete[] brojevi;
    brojevi = nullptr;
    velicina = 0;
  }

  MojNizIntova(const MojNizIntova& drugi): velicina(drugi.velicina) {
    std::cout << "Konstruktor kopije MojNizIntova(const MojNizIntova& drugi)\n";
    brojevi = new int[velicina];
    for (int i = 0; i < velicina; ++i)
      brojevi[i] = drugi.brojevi[i]; 
  }

  MojNizIntova(MojNizIntova&& drugi): brojevi(drugi.brojevi), velicina(drugi.velicina) {
    std::cout << "Konstruktor pomjeranja MojNizIntova(MojNizIntova&& drugi)\n";
    drugi.brojevi = nullptr;
    drugi.velicina = 0;
  }

  MojNizIntova& operator=(const MojNizIntova& drugi) {
    std::cout << "Operator dodjele MojNizIntova& operator=(const MojNizIntova& drugi)\n";
    if (this == &drugi) return *this;

    // Brisanje starog
    delete[] brojevi;

    // Prepisivanje preko starog
    velicina = drugi.velicina;
    // Alokacija novog
    brojevi = new int[velicina];

    for (int i = 0; i < velicina; ++i)
      brojevi[i] = drugi.brojevi[i];

    return *this;
  }

  MojNizIntova& operator=(MojNizIntova&& drugi) {
    std::cout << "Operator dodjele pomjeranja MojNizIntova& operator=(MojNizIntova&& drugi)\n";
    if (this == &drugi) return *this;

    // Brisanje starog
    delete[] brojevi;

    // Prepisivanje preko starog
    brojevi = drugi.brojevi;
    velicina = drugi.velicina;

    // Postavljanje drugog na prazan da ga destruktor ne obrise
    drugi.brojevi = nullptr;
    drugi.velicina = 0;

    return *this;
  }


  int get(int index) const { return brojevi[index]; }
  void set(int index, int value) { brojevi[index] = value; }
  int dajVelicinu() const { return velicina; }
  void ispisi() const {
    for (int i = 0; i < velicina; ++i)
      std::cout << brojevi[i] << " ";
    std::cout << std::endl;
  }
};

int main() {
  MojNizIntova niz1({1, 2, 3, 4, 5});
  niz1.set(0, 10);

  MojNizIntova niz2(niz1); // Poziva se konstruktor kopije
  niz2.ispisi(); // Očekivani ispis: 10 2 3 4 5

  MojNizIntova niz3(std::move(niz1)); // Poziva se konstruktor pomjeranja
  niz3.ispisi(); // Očekivani ispis: 10 2 3 4 5

  // std::cout << niz1.get(2); // Krahira, jer je niz1 sada prazan nakon pomjeranja

  niz2 = niz3; // Poziva se operator dodjele kopije
  niz2.ispisi(); // Očekivani ispis: 10 2 3 4 5
  
  niz3 = std::move(niz2); // Poziva se operator dodjele pomjeranja
  niz3.ispisi(); // Očekivani ispis: 10 2 3 4 5

  // std::cout << niz2.get(2); // Krahira, jer je niz2 sada prazan nakon pomjeranja
}
