#include <iostream>
#include <vector>

std::vector<int> Divisors(int n) {
    std::vector<int> res;
    for(int i = 1; i <= n; i++)
        if(n % i == 0) res.push_back(i);
    return res;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> divisors = Divisors(n);
    std::cout << "Djelioci su: ";
    for(int i = 0; i < divisors.size(); i++)
        std::cout << divisors[i] << " ";
    return 0;
}