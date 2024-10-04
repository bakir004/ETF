#include <cmath>
#include <complex>
#include <iomanip>
#include <ios>
#include <iostream>
#include <vector>

typedef std::complex<int> Kompleksni;

bool Prost(int n) {
  if (n == 1)
    return true;
  int korijen = sqrt(n);
  for (int i = 2; i <= korijen; i++)
    if (n % i == 0)
      return false;
  return true;
}

std::vector<Kompleksni> KompleksniBrojeviBlizanci(int n) {
  std::vector<Kompleksni> kompleksni;
  int brojac = 1;
  while (n) {
    if (Prost(brojac) && Prost(brojac + 2)) {
      kompleksni.push_back(Kompleksni(brojac, brojac + 2));
      n--;
    }
    brojac++;
  }
  return kompleksni;
}

int main() {
  int n;
  while (true) {
    std::cout << "Unesite kolicinu blizanaca kompleksnih brojeva: ";
    std::cin >> n;
    if (!(std::cin) || n <= 0) {
      std::cout << "Uneseni broj mora biti prirodan\n";
      std::cin.clear();
      std::cin.ignore(10000, '\n');
    } else
      break;
  }

  std::vector<Kompleksni> kompleksni = KompleksniBrojeviBlizanci(n);
  for (Kompleksni z : kompleksni)
    std::cout << std::setw(3) << std::right << z.real() << " + i"
              << std::setw(3) << std::left << z.imag() << "\n";

  return 0;
}