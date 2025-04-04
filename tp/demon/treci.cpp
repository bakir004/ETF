#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
  int xmin, xmax;
  std::cout << "Unesite pocetnu i krajnju vrijednost: ";
  std::cin >> xmin >> xmax;
  std::cout << "\n"
            << "+---------+----------+----------+-----------+\n"
            << "| Brojevi | Kvadrati | Korijeni | Logaritmi |\n"
            << "+---------+----------+----------+-----------+\n";

  for (int x = xmin; x <= xmax; x++)
    std::cout << "| " << std::left << std::setw(8) << x << "|" << std::right
              << std::setw(9) << x * x << " |" << std::fixed << std::setw(9)
              << std::setprecision(3) << std::sqrt(x) << " |" << std::setw(10)
              << std::setprecision(5) << std::log(x) << " |\n";
  std::cout << "+---------+----------+----------+-----------+\n";
  return 0;
}
