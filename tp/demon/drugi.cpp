#include <cmath>
#include <iostream>

int main() {
  double a, b, c;
  std::cout << "Unesite tri broja: ";
  std::cin >> a >> b >> c;
  if (a <= 0 || b <= 0 || c <= 0 || a + b <= c || a + c <= b || b + c <= a)
    std::cout << "Ne postoji trougao cije su duzine stranica " << a << ", " << b
              << " i " << c << "!\n";
  else {
    double s = (a + b + c) / 2;
    double p = std::sqrt(s * (s - a) * (s - b) * (s - c));
    double alfa = std::acos((b * b + c * c - a * a) / (2 * b * c));
    double beta = std::acos((a * a + c * c - b * b) / (2 * a * c));
    double gama = std::acos((a * a + b * b - c * c) / (2 * a * b));
    double minugao;
    if (alfa < beta && alfa < gama)
      minugao = alfa;
    else if (beta < gama)
      minugao = beta;
    else
      minugao = gama;
    minugao *= 45 / std::atan(1);
    double minute = 60 * (minugao - int(minugao));
    double sekunde = 60 * (minute - int(minute));
    std::cout << "Obim trougla sa duzinama stranica " << a << ", " << b << " i "
              << c << " iznosi " << 2 * s << ".\n"
              << "Njegova povrsina iznosi " << p << ".\n"
              << "Njegov najmanji ugao ima " << int(minugao) << " stepeni, "
              << int(minute) << " minuta i " << int(sekunde) << " sekundi.\n";
  };
  return 0;
}
