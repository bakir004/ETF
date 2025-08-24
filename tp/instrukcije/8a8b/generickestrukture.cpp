#include<iostream>

const int velicinaNiza = 20;

template <typename NekiTip>
struct ProizvoljniNiz {
  NekiTip elementi[velicinaNiza];
};

int main() {
  // int a;
  // std::cin >> a;
  // const int b = a;

  ProizvoljniNiz<int> niz;
  for(int i = 0; i < velicinaNiza; i++) {
    niz.elementi[i] = 0;
  }
  niz.elementi[2] = 2;

  // for(int i = 0; i < velicinaNiza; i++) {
  //   std::cout << niz.elementi[i] << " ";
  // }
  ProizvoljniNiz<std::string> nizStringova;
  for(int i = 0; i < velicinaNiza; i++) {
    nizStringova.elementi[i] = ".";
  }
  nizStringova.elementi[3] = "Hello";
  nizStringova.elementi[4] = "World";

  for(int i = 0; i < velicinaNiza; i++) {
    std::cout << nizStringova.elementi[i] << " ";
  }

  // ProizvoljniNiz niz; OVO NE MOZE

  return 0;
}
