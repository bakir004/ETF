#include <iostream>
#include <complex>
#include <cmath>

typedef std::complex<double> Complex;

int main() {
    double x,n;
    std::cin >> x >> n;
    Complex k1 = pow(Complex(cos(x), sin(x)), n);
    Complex k2 = Complex(cos(n*x), sin(n*x));
    std::cout << k1 << " = " << k2;
    return 0;
}