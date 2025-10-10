#include <iostream>
#include <iomanip>
#include <complex>
#include <string>

void P1(int a, int b) {
    std::cout << a << b;
    a += 3; b *= 2;
    std::cout << a << b;
}
void P2(int &a, int &b) {
    std::cout << a << b;
    a += 3; b *= 2;
    std::cout << a << b;
}
int F1(int x) {
    return x + 1;
}
int F2(int x) {
    return 2 * x;
}
int main() {
    int a(2), b(5); // a = 2, b = 5
    std::cout << 3 << std::setw(3) << a << a * a << std::setw(5)
        << b * b << std::endl;
    // 3  24    25
    std::complex<double> c = 3, i(0, 1); // format ispisa complex je (re,im)
    std::cout << c << c * c << " " << c * i << c + c * i << std::endl;
    // (3,0)(9,0) (0,3)(3,3)
    P1(b, a); // poziv je P1(5,2)
    std::cout << a << b << std::endl;
    // 528425
    P2(b, a); // poziv je P2(5,2) al je sad po referenci
    std::cout << a << b << std::endl; // promjene u P2 nad varijablama a i b su zamijenjene
    // 528448
    std::string s = "abcdefghijklmnopqrstuvwxyz";
    std::cout << s.length() << s.size() << s.substr(0, 5) + s.substr(10, 3) << std::endl;
    // 2626abcdeklm
    int (*npf[5])(int) = {F1, F2, F1, F2, F2};
    int suma(0);
    for(int i = 0; i < 5; i++) suma += (i + 1) * npf[i](i);
    // suma += 1 * 1;
    // suma += 2 * 2
    // suma += 3 * 3
    // suma += 4 * 6
    // suma += 5 * 8
    // 78
    std::cout << suma;
    return 0;
}