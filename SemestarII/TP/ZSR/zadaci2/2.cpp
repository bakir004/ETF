#include <iostream>
#include <vector>

bool HasEqualDigits(int n) {
    std::vector<bool> digits(10, false);
    while(n > 0) {
        if(digits[n % 10]) return true;
        digits[n % 10] = true;
        n /= 10;
    }
    return false;
}

int main() {
    int n;
    std::cin >> n;
    std::cout << "Ima jednake cifre: " << std::boolalpha << HasEqualDigits(n);
    return 0;
}