#include<iostream>
#include<fstream>

int main() {
  // std::fstream binarni("brojevi.dat", std::ios::out | std::ios::binary);
  // for(double i = -5; i <= 5; i++) {
  //   binarni.write(reinterpret_cast<char*>(&i), sizeof i);
  // }
  // binarni.close();

  std::fstream binarni("brojevi.dat", std::ios::in | std::ios::binary);
  if(!binarni) {
    std::cout << "Ne valja datoteka\n";
    return 1;
  }

  // binarni.seekg(0, std::ios::end);
  // int brojElemenata = binarni.tellg() / sizeof(double);

  double trenutni;
  std::vector<double> elementi;
  while(binarni.read(reinterpret_cast<char*>(&trenutni), sizeof trenutni)) {
    elementi.push_back(trenutni);
  }
  if(!binarni.eof()) {
    std::cout << "Problem sa citanjem iz datoteke\n";
  }
  binarni.close();
  binarni = std::fstream("brojevi.dat", std::ios::out | std::ios::binary);

  std::ofstream tekstualna("pozitivni.txt");
  for(int i = 0; i < elementi.size(); i++) {
    if(elementi[i] < 0) elementi[i] = 0;
    if(elementi[i] > 0)
      tekstualna << elementi[i] << " ";
  }
  tekstualna.close();

  for(int i = 0; i < elementi.size(); i++) {
    binarni.write(reinterpret_cast<char*>(&elementi[i]), sizeof elementi[i]);
  }
  binarni.close();


  return 0;
}
