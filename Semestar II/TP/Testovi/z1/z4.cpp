
// TP 2024/2025: Zadaća 1, Zadatak 4
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

std::array<std::vector<std::string>, 100>
RazvrstajRijeciPoDuzini(std::string s) {
  std::array<std::vector<std::string>, 100> rez;
  int duzina = s.length();
  int br_rijeci=0;
  for (int i = 0; i < duzina; i++) {
    if (std::isalnum(s.at(i)) && (i == 0 || !std::isalnum(s.at(i-1)))) {
      std::string rijec;
      while (i < duzina && std::isalnum(s.at(i))) {
        rijec += std::toupper(s.at(i));
        i++;
      }
      br_rijeci++;
      if(rijec.length()>=100 && br_rijeci>1) throw std::range_error("Recenica sadrzi predugacku rijec!");
      if (rijec.length() >= 100) throw std::range_error("Predugacka rijec!");
         
      rez.at(rijec.length()).push_back(rijec);
    }
    
  }
  return rez;
}

int main() {
  std::string s;
  std::cout << "Unesite recenicu: ";
  std::getline(std::cin, s);
  try {
    auto rez = RazvrstajRijeciPoDuzini(s);
    for (int i = 0; i < 100; i++) {
      if (!rez.at(i).empty()) {
        std::cout << "Duzina " << i << ": ";
        for (int j = 0; j < rez.at(i).size(); j++) {
          std::cout << rez.at(i).at(j)<< " ";
        }
        std::cout << std::endl;
      }
    }

  } catch (std::range_error greska) {
    std::cout << "Greska: " << greska.what();
  }

  return 0;
}
