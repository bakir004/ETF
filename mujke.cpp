#include <iostream>
#include <vector>
#include <list>

std::vector<int> Razbrajanje(int N, int M) {
  std::vector<int> distrikti;
  std::list<int> lista;
  for (int i = 1; i <= N; i++) {
    lista.push_back(i);
  }
  int brojac = 0;
  auto it = lista.begin();
  while (brojac < N) {
    distrikti.push_back(*it);
    it = lista.erase(it);
    if (it == lista.end())
      it = lista.begin();

    int zaPomjeriti = M-1;
    if(std::distance(it, lista.end()) <= M - 1) {
      zaPomjeriti -= (std::distance(it, lista.end()));
      it = lista.begin();
    }
    std::advance(it, zaPomjeriti);
    brojac++;
    //    V          
    //  2 4 6 8 10 .

    // auto pomocni = it;
    // int br = 0;
    // while (pomocni != lista.end()) {
    //   pomocni++;
    //   br++;
    // }
    // if (br < M - 1) {
    //   it = lista.begin();
    //   std::advance(it, M - 1 - br);
    // } else
    //   std::advance(it, M - 1);
    // brojac++;
  }
  return distrikti;
}

int OdabirKoraka(int N, int K) {
  for (int i = 1; i <= N; i++) {
    auto v = Razbrajanje(N, i);
    if (v[v.size() - 1] == K)
      return i;
  }
  return 0;
}

int main() {
  auto v = Razbrajanje(10, 2);
  std::cout << std::endl;
  for (int i = 0; i < v.size(); i++) {
    std::cout << v[i] << " ";
  }
  int N,K;
  std::cout << "Unesite broj distrikta u gradu: ";
  std::cin >> N;
  std::cout << "Unesite redni broj distrikta u kojem se nalazi restoran: ";
  std::cin >> K;
  
  std::cout << "Trazeni korak: " << OdabirKoraka(N, K);
  return 0;
}