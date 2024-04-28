// TP 2023/2024: LV 1, Zadatak 1
#include <iomanip>
#include <iostream>

bool validDate(int d, int m, int y) {
  return !(d < 1 || d > 31 || m > 12 || m < 1 || y < 0);
}

bool isLeapYear(int y) { return y % 400 == 0 || (y % 100 != 0 && y % 4 == 0); }

int dayInYear(int d, int m, int y) {
  int day = d + ((m > 2) ? isLeapYear(y) : 0);
  switch (m) {
  case 2:
    day += 31;
    break;
  case 3:
    day += 59;
    break;
  case 4:
    day += 90;
    break;
  case 5:
    day += 120;
    break;
  case 6:
    day += 151;
    break;
  case 7:
    day += 181;
    break;
  case 8:
    day += 212;
    break;
  case 9:
    day += 243;
    break;
  case 10:
    day += 273;
    break;
  case 11:
    day += 304;
    break;
  case 12:
    day += 334;
    break;
  }
  return day;
}

int main() {
  int d, m, y;
  char p1, p2;
  double degrees;
  std::cout << "Unesite datum u formatu DD.MM.YYYY i geografsku sirinu u "
               "stepenima (izmedju -90° i 90°): ";
  if (std::cin >> d >> p1 >> m >> p2 >> y && p1 == '.' && p2 == '.' &&
      (std::cin >> degrees) && degrees >= -90 && degrees <= 90 &&
      validDate(d, m, y)) {
    int dayOfyear = dayInYear(d, m, y);
    double resInHours =
        18 + 4 * double(dayOfyear) / (365 + isLeapYear(y)) - 2 * degrees / 90.;

    int totalSeconds = resInHours * 3600;
    int hours = totalSeconds / 3600;
    totalSeconds = totalSeconds % 3600;
    int minutes = totalSeconds / 60;
    totalSeconds = totalSeconds % 60;
    int seconds = totalSeconds;

    std::cout << "Ocekivano vrijeme zalaska sunca na " << dayOfyear
              << ". dan je " << std::setw(2) << std::setfill('0') << hours
              << ":" << std::setw(2) << std::setfill('0') << minutes << ":"
              << std::setw(2) << std::setfill('0') << seconds << ".";
  } else
    std::cout << "Neispravan unos";

  return 0;
}
