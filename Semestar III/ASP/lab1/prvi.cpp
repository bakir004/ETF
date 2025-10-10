#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <ctime>
#define UPPER_BOUND 200000001

bool prime[UPPER_BOUND];

void Sieve(int n) {
    std::fill(prime, prime + n, true);
    prime[0] = prime[1] = false;
    
    for(int i = 2; i*i <= n; i++) {
        if(!prime[i]) continue;
        for(int j = i*i; j <= n; j+=i) 
            prime[j] = false;
    }
}

void Goldbach(int n, int& p, int& q) {
    if(n % 2 != 0 || n <= 2) throw std::domain_error("Greska"); 
    Sieve(n);
    for(int i = 1; i <= UPPER_BOUND/2; i+=2) {
        if(prime[i] && prime[n-i]) {
            p = i;
            q = n-i;
            return;
        }
    }
    throw std::domain_error("Greska");
}

int main() {
    int n = 100000000;
    int p, q;
    clock_t time1 = clock();
    Goldbach(n, p, q);
    clock_t time2 = clock();
    std::cout << p << " " << q;
    int vrijeme = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "\nTime: " << vrijeme << "ms";
    return 0;
}
