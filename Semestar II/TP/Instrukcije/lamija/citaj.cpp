#include<iostream>
#include<fstream>

int main() {
  double trenutni;
  std::ifstream binarni("brojevi.dat");
  while(binarni.read(reinterpret_cast<char*>(&trenutni), sizeof trenutni)) {
    std::cout << trenutni << " ";
  }
  return 0;
}
