#include <cmath>
#include <iostream>

double Simpson(double f(double), double a, double b, int n) {
    double h = (b-a)/n;
    double suma1 = 0, suma2 = 0;
    for(int k = 1; k <= n - 1; k+=2)
        suma1 += f(a + k * h);
    for(int k = 2; k <= n - 2; k+=2)
        suma2 += f(a + k * h);
    return (h/3)*(f(a) + 4 * suma1 + 2 * suma2 + f(b));
}

int main() {
    std::cout << Simpson(std::sin, 0, 4*std::atan(1), 22) << "\n"; // 2
    std::cout << Simpson([](double x){ return 1/x; }, 1, 2, 28) << "\n"; // ln(2) = 0.69314
    std::cout << Simpson([](double x){ return x*x*x; }, 0, 10, 2) << "\n"; // 2500
    return 0;
}