#include <iostream>

double Denominator(int curr, int bound) {
    if(curr == bound-1) return 1./(curr + 1./bound);
    return 1./(curr + Denominator(curr+1, bound));
}

int main() {
    int n;
    std::cin >> n;
    std::cout << Denominator(1, n);
    return 0;
}