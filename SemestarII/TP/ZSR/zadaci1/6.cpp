#include <iostream>
#define EPSILON 0.0001

int main()
{
    double a, b, c;
    std::cin >> a >> b >> c;
    if (a * a + b * b - c * c < EPSILON || c * c + b * b - a * a < EPSILON || a * a + c * c - b * b < EPSILON)
        std::cout << "Pravougli";
    else
        std::cout << "Nije pravougli";
    return 0;
}