#include<iostream>
#include<fstream>

int main() {
  std::ifstream binarna("brojevi.dat", std::ios::binary);
  double broj;
  while(binarna.read(reinterpret_cast<char*>(&broj), sizeof(double))) {
    std::cout << broj << " ";
  }

  return 0;
}
