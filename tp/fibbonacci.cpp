#include <cmath>
#include <iostream>
#include <iterator>

long long int memo[1000];

long long int fib(int n) {
    if(n==0) return 0;
    if(n==1) return 1;
    if(memo[n] == -1) return memo[n] = fib(n-1)+fib(n-2);
    return memo[n];
}

int main() {
    std::fill(std::begin(memo), std::end(memo), -1);
    memo[0] = 0;
    memo[1] = 1;
    long long int memo2[1000];
    std::fill(std::begin(memo2), std::end(memo2), -1);
    memo2[0] = 0;
    memo2[1] = 1;
    memo2[2] = 1;
    double phi = (1+std::sqrt(5))/2;
    for(int i = 0; i < 1000; i++) {
        long long int f1 = fib(i);
        
        long long int f2 = 1;
        if(memo2[i] != -1) f2 = memo2[i];
        else f2 = memo2[i] = std::round(memo2[i-1] * phi);
        std::cout << f1 << " " << f2 << '\n';
        if(f1 != f2) {
            std::cout << "Failed at: " << f1 << " " << f2 << '\n';
            break;
        }
    }
    return 0;
}
