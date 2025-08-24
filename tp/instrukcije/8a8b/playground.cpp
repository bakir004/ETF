#include<iostream>

struct Ucenik {
  std::string ime;
  double prosjek;
};

struct Tacka {
  double x,y;
};

Ucenik vratiUcenika() {
  return {"Haso", 5};
}

int main() {
  // std::vector<std::string> imenaUcenika{"Bakir", "Elmir", "Ajla"};
  // std::vector<double> prosjeciUcenika{9, 10, 9.5};
  //
  // for(int i = 0; i < imenaUcenika.size(); i++) {
  //   std::cout << i << ". ucenik:\n" << imenaUcenika[i] << ", " << prosjeciUcenika[i] << "\n";
  // }

  // Ucenik prvi{"Bakir", 9};
  // Ucenik drugi{"Elmir", 10};
  // Ucenik treci{"Ajla", 9.5};

  // std::cout << drugi.prosjek;
  // struktura se moze poistovijetiti sa inicijalizacijskom listom
  // Ucenik meho = {"Meho", 7};

  // Ucenik ucenici[4]{ {"Bakir", 9}, {"Elmir", 10}, {"Ajla", 9.5}, vratiUcenika() };
  //
  // for(auto ucenik : ucenici) {
  //   std::cout << "Ucenik " << ucenik.ime << ", prosjek: " << ucenik.prosjek << "\n";
  // }

  // Ucenik* ucenik1 = new Ucenik{"Mujo", 8};
  // Ucenik* ucenik2 = new Ucenik{"Haso", 8};
  // Ucenik* ucenik3 = new Ucenik{"Huso", 8};
  //
  // std::vector<Ucenik*> ucenici{ ucenik1, ucenik2, ucenik3 };
  //
  // for(auto ucenik : ucenici) { // ucenik je tipa Ucenik*
  //   std::cout << "Ucenik " << ucenik->ime << ", prosjek: " << ucenik->prosjek << "\n";
  //   // -> je skracenica za dereferenciranje PA ONDA pristupanje atributu
  // }

  // std::pair<std::string, double> ucenik1{"Mujo", 7};
  //
  // Tacka tacka{3.42, 9.11};
  //
  // std::cout << ucenik1.first << ", " << ucenik1.second << "\n";
  // std::cout << tacka.x << ", " << tacka.y << "\n";


  return 0;
}
