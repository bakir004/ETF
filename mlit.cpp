#include <iostream>

int fromPairToInt1(int a, int b) {
    return (a+b-2)*(a+b-1)/2 + a;
}

int fromPairToInt2(int a, int b) {
    return (2*a + b - 2);
}

int main() {
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            std::cout << fromPairToInt1(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            std::cout << fromPairToInt2(i, j) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
