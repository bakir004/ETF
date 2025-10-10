#include <iostream>

int main() {
    int num;
    std::cin >> num;
    int numCopy = num;
    int reversed = 0;
    while(num > 0) {
        int digit = num % 10;
        num /= 10;
        reversed = reversed * 10 + digit;
    }
    if(reversed == numCopy)
        std::cout << "Palindroman";
    else
        std::cout << "Nije palindroman";
    return 0;
}