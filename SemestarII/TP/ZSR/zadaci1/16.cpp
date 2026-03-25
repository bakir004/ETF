#include <iostream>

int DigitProduct(int n) {
    int product = 1;
    while(n > 0) {
        int digit = n%10;
        n /= 10;
        product *= digit;
    }
    return product;
}

int MultOtpornost(int n) {
    int counter = 0;
    while(n / 10 > 0) {
        n = DigitProduct(n);
        counter++;
    }
    return counter;
}

int main() {
    int a, b, k;
    std::cin >> a >> b >> k;
    for(int i = a; i <= b; i++) {
        if(MultOtpornost(i) == k)
            std::cout << i << " ";
    }
    return 0;
}