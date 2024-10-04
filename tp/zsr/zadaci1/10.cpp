#include <iostream>

int main()
{
    int num;
    int counter = 2;
    std::cin >> num;
    while (num > 1) {
        if (num % counter == 0)
        {
            std::cout << counter << " ";
            while (num % counter == 0)
                num /= counter;
        }
        counter++;
    }
    return 0;
}