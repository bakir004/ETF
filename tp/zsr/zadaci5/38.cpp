#include <algorithm>
#include <iostream>
#include <complex>
#include <cmath>
#include <deque>

bool CompareComplex(const std::complex<double> &c1, const std::complex<double> &c2) {
    if(abs(c1) != abs(c2)) return abs(c1) < abs(c2);
    return c1.real() < c2.real();
}

int main() {
    std::deque<std::complex<double>> dek{{3,4},{4,5},{1,2},{4,3}};
    std::sort(dek.begin(),dek.end(),CompareComplex);
    for(auto c : dek)
        std::cout << c << " ";
    return 0;
}