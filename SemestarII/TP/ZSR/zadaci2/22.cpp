#include <iostream>
#include <vector>

std::vector<int> PrimeFactorisation(int n) {
    std::vector<int> primeFactors;
    int divisor = 2;
    while(n > 1) {
        if(n % divisor == 0) {
            n /= divisor;
            primeFactors.push_back(divisor);
        } else divisor++;
    }
    return primeFactors;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> primeFactors = PrimeFactorisation(n);
    for(int i = 0; i < primeFactors.size(); i++)
        std::cout << primeFactors[i] << " ";

    return 0;
}