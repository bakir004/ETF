#include <iostream>

int main()
{
    int num;
    bool odd = false, positive = true;
    for (int i = 0; i < 6; i++) {
        std::cin >> num;
        if (num < 0)
            positive = false;
        if (num % 2 == 1)
            odd = true;
    }
    // ispravljeno
    if (positive)
        std::cout << "Svi pozitivni\n";
    if (odd)
        std::cout << "Ima neparni/h";
    return 0;
}