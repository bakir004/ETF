#include <iostream>
#include <iomanip>

int divisorSum(int n) {
    int sum = 0;
    for(int i = 1; i <= n; i++)
        if(n % i == 0) sum += i;
    return sum;
}

int main() {
    int n1, n2;
    std::cin >> n1 >> n2;
    std::cout << "+------+------+\n";
    for(int i = n1; i <= n2; i++) {
        std::cout << "| " << std::setw(4) << i << " | " << std::setw(4) << divisorSum(i) << " |\n";
        std::cout << "+------+------+\n";
    }
    
    return 0;
}