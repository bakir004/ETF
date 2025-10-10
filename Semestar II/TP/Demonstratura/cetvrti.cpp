#include <iostream>
int main() {
  double x;
  do {
    std::cout << "Unesite prirodan broj ili 0 za kraj: ";
    std::cin >> x;
    if (!std::cin || x < 0 || int(x) != x) {
      std::cout << "Niste unijeli prirodan broj!\n";
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      x = 1; // Sprečava prekid petlje!
    } else if (x != 0) {
      int suma = 0;
      for (int i = 1; i <= int(x) / 2; i++)
        if (int(x) % i == 0)
          suma += i;
      std::cout << "Broj " << x << " je ";
      if (suma < x)
        std::cout << "manjkav!\n";
      else if (suma > x)
        std::cout << "obilan!\n";
      else
        std::cout << "savrsen!\n";
    }
  } while (x != 0);
  std::cout << "Dovidjenja!\n";
  return 0;
}
