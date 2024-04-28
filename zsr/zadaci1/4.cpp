#include <iostream>

int main()
{
    int sati;
    std::cin >> sati;
    double pretplata1 = 10 + sati * 0.5;
    double pretplata2 = sati * 0.8;
    if (pretplata1 < pretplata2)
        std::cout << "Prva firma";
    else
        std::cout << "Druga firma";
    return 0;
}