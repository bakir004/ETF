#include <iostream>

int main()
{
    double x;
    int n;
    std::cin >> x >> n;
    double S = 0;
    short current = -1;
    for (int k = 1; k <= n; k++) {
        S += current / (x * (x + k));
        current *= -1;
    }
    std::cout << S;
    return 0;
}