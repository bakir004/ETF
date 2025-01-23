#include <iostream>
#include <cmath>

const double PI = 4*std::atan(1);

double sum(int n) {
    double res = 0;
    for(int k = 0; k <= n; k++) {
        res += std::pow(2,k)*std::sin((n-k)*PI/3);
    }
    return res;
}

double mojSum(int n) {
    return std::sqrt(3) * (std::pow(2,n)-std::cos(n*PI/3)) / 3.;
}

int main() {
    for(int n = 1; n <= 30; n++) {
        std::cout << sum(n) << ", " << mojSum(n) << '\n';
    }
    return 0;
}
