#include <initializer_list>
#include<iostream>

class NekaKlasa {
  int x, y;
public:
  // NekaKlasa() {
  //   x = y = 0;
  // }
  // NekaKlasa(int x) {
  //   this->x = x;
  //   this->y = x;
  // }
  // NekaKlasa(int x, int y) {
  //   this->x = x;
  //   this->y = y;
  // }
  // explicit NekaKlasa(int x = 0, int y = 0) {
  //   std::cout << "Poziv konstruktora\n";
  //   this->x = x;
  //   this->y = y;
  // }
  explicit NekaKlasa(int x, int y) {
    std::cout << "Poziv eksplicitnog konstruktora\n";
    this->x = x;
    this->y = y;
  }
  // NekaKlasa(const NekaKlasa& k) {
  //   std::cout << "Kopirajuci konstruktor\n";
  //   this->x = k.x;
  //   this->y = k.y;
  // }
  // NekaKlasa(const NekaKlasa& k) = delete;
  // NekaKlasa(std::initializer_list<int> lista) {
  //   std::cout << "Sekvencijalni konstruktor\n";
  // }
  void Ispisi() {
    std::cout << "x: " << x << ", y: " << y << "\n";
  }
};

int main() {
  // pitanje: ima li poente staviti "explicit" na konstruktor sa vise od jednog parametra? IMA
  // NekaKlasa a(3,4);
  // NekaKlasa b{3,4};
  // NekaKlasa c = {3,4};

  // std::vector<int> v(3);
  // std::vector<int> v2 = 3; // ovo ne radi jer je vektorov konstruktor sa jednim parametrom explicit
  //
  // NekaKlasa b = 3;
  // NekaKlasa c{1,3};
  // NekaKlasa d = {1,3};
  // c.Ispisi(); 
  // d.Ispisi();

  // std::vector<NekaKlasa> v;
  // std::vector<NekaKlasa> v{NekaKlasa(3,5), NekaKlasa(9,3)};
  // v.push_back(NekaKlasa(3,5));
  // v.push_back(NekaKlasa(9,3));
  // NekaKlasa k2(3,5);
  // v.push_back(k2);

  // for(int i = 0; i < 5; i++) {
  //   // NekaKlasa k(i,i+1);
  //   // v.push_back(k);
  //   v.emplace_back(i, i+1);
  // }

  // for(int i = 0; i < v.size(); i++) {
  //   v[i].Ispisi();
  // }
  // NekaKlasa d{1};
  // d.Ispisi();
  // NekaKlasa k1(4,7);
  // k1.Ispisi();
  // NekaKlasa k2(k1); 
  // k2.Ispisi();

  // std::vector<int> v(5,0); // konstruktor vektora
  // std::vector<int> v2(5); // konstruktor vektora

  // NekaKlasa a(5);
  // NekaKlasa b = 5;
  // b.Ispisi();
  // NekaKlasa c = NekaKlasa(5);
  // NekaKlasa d(NekaKlasa(5));

  // std::vector<int> v(4,2); // vektor sa 4 dvice
  // std::vector<int> v2{4,2}; // 2 elementa: 4 i 2

  return 0;
}
