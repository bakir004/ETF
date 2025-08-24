#include<iostream>
#include<fstream>

int main() {
  // std::ifstream ulazna("brojevi.txt");
  // std::ofstream izlazna("brojevi.dat", std::ios::binary);
  // double broj;
  // while(ulazna >> broj) {
  //   izlazna.write(reinterpret_cast<char*>(&broj), sizeof broj);
  // }
  // if(!ulazna.eof()) {
  //   std::cout << "Greska pri citanju\n";
  //   return 1;
  // }
  // ulazna.close();
  // izlazna.close();

  // for(int i = 0; i < v.size()/2; i++) {
  //   std::swap(v[i], v[v.size()-1-i]);
  // }

  std::fstream binarna("brojevi.dat", std::ios::in | std::ios::out | std::ios::binary);

  binarna.seekg(0, std::ios::end);
  int velicina = binarna.tellg() / sizeof(double);

  for (int i = 0; i < velicina / 2; i++) {
    double n1, n2;

    binarna.seekg(i * sizeof(double), std::ios::beg);
    binarna.read(reinterpret_cast<char*>(&n1), sizeof(double));

    binarna.seekg(-((i + 1) * sizeof(double)), std::ios::end);
    binarna.read(reinterpret_cast<char*>(&n2), sizeof(double));

    binarna.seekp(i * sizeof(double), std::ios::beg);
    binarna.write(reinterpret_cast<char*>(&n2), sizeof(double));

    binarna.seekp(-((i + 1) * sizeof(double)), std::ios::end);
    binarna.write(reinterpret_cast<char*>(&n1), sizeof(double));
  }
  std::cout << "Uspjesno obrnut redoslijed!\n";

  binarna.close();
  return 0;
}
