// TP 2024/2025: Zadaća 1, Zadatak 3
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include<limits>

enum class Smjer { LijevaSpiralna, DesnaSpiralna, NijeSpiralna };


std::vector<std::vector<int>> KreirajSpiralnuMatricu(int m, int n, int k,
                                                     Smjer smjer) {
  std::vector<std::vector<int>> mat(m, std::vector<int>(n, 0));
  int broj = k;
  int lijevo = 0, desno = n - 1, dole = m - 1, gore = 0;
  int max_br=std::numeric_limits<int>::max();
  if (smjer == Smjer::DesnaSpiralna) {
    while (lijevo <= desno && gore <= dole) {
      for (int i = lijevo; i <= desno; i++){
        mat.at(gore).at(i) = broj;
        if(broj<max_br) broj++;}
      gore++;

      for (int i = gore; i <= dole; i++) {
        mat.at(i).at(desno) = broj;
        if(broj<max_br) broj++;}
      desno--;

      if (gore <= dole) {
        for (int i = desno; i >= lijevo; i--){
          mat.at(dole).at(i) = broj;
          if(broj<max_br) broj++;}
        dole--;
      }
      if (lijevo <= desno) {
        for (int i = dole; i >= gore; i--){
          mat.at(i).at(lijevo) = broj;
          if(broj<max_br) broj++;
        }
        lijevo++;
      }
    }

  } else {
    while (lijevo <= desno && gore <= dole) {
      for (int i = gore; i <= dole; i++){
        mat.at(i).at(lijevo) = broj;
        if(broj<max_br) broj++;}
      lijevo++;
      for (int i = lijevo; i <= desno; i++){
        mat.at(dole).at(i) = broj;
        if(broj<max_br) broj++;}
      dole--;
      if (lijevo <= desno) {
        for (int i = dole; i >= gore; i--){
          mat.at(i).at(desno) = broj;
          if(broj<max_br) broj++;}

        desno--;
      }
      if (gore <= dole) {
        for (int i = desno; i >= lijevo; i--) {
          mat.at(gore).at(i) = broj;
          if(broj<max_br) broj++;}
        gore++;
      }
    }
  }
  
  return mat;
}

Smjer DaLiJeSpiralnaMatrica(std::vector<std::vector<int>> mat) {
  if (mat.at(0).size() == 0)
    return Smjer::NijeSpiralna;

  int m = mat.size(), n = mat.at(0).size();
  auto desna =
      KreirajSpiralnuMatricu(m, n, mat.at(0).at(0), Smjer::DesnaSpiralna);
  auto lijeva =
      KreirajSpiralnuMatricu(m, n, mat.at(0).at(0), Smjer::LijevaSpiralna);

  if (mat == desna)
    return Smjer::DesnaSpiralna;
  if (mat == lijeva)
    return Smjer::LijevaSpiralna;
  return Smjer::NijeSpiralna;
}

void IspisiMatricu(std::vector<std::vector<int>> m) {
  int max_broj = m.at(0).at(0);
  for (auto red : m) {
    for (int x : red) {
      if (x > max_broj)
        max_broj = x;
    }
  }
  int sirina = std::to_string(max_broj).length() + 1;

  for (auto red : m) {
    for (int x : red) {
      std::cout << std::setw(sirina) << x << " ";
    }
    std::cout << std::endl;
  }
}

int main() {
  int m, n;
  std::cout << "Unesite broj redova i kolona matrice: ";
  std::cin >> m >> n;

  int k;
  std::cout << "Unesite pocetnu vrijednost: ";
  std::cin >> k;
  char oznaka;
  std::cout << "Unesite L za lijevu, a D za desnu spiralnu matricu: ";
  std::cin >> oznaka;

  Smjer smjer = (oznaka == 'D') ? Smjer::DesnaSpiralna : Smjer::LijevaSpiralna;

  auto matrica = KreirajSpiralnuMatricu(m, n, k, smjer);

  std::cout << "\nKreirana "
            << ((smjer == Smjer::DesnaSpiralna) ? "desna" : "lijeva")
            << " spiralna matrica:\n";
  if (m <= 0 || n <= 0)
    return 0;

  if (m > 0)
    IspisiMatricu(matrica);

  return 0;
}
