#include <iostream>
#define MORSKA_MILJA_U_METRIMA 1852

int main()
{
    int cvorovi;
    std::cin >> cvorovi;
    double kmh = MORSKA_MILJA_U_METRIMA * (double)cvorovi / 1000;
    std::cout << kmh;
    return 0;
}