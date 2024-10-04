#include <iostream>

int main() {
    double x;
    int N;
    std::cin >> x >> N;
    double coefficient;
    std::cin >> coefficient;
    double res = coefficient;
    for(int i = 1; i < N; i++) {
        std::cin >> coefficient;
        res = res*x + coefficient;
    }
    std::cout << res;
    return 0;
}