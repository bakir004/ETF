#include <cstdlib>
#include<iostream>
#include<vector>

void Sieve(int n) {
    if(n <= 0) return;
    std::vector<int> prime(n, true);
    prime[0] = prime[1] = false;
    
    for(int i = 2; i*i <= n; i++) {
        if(!prime[i]) continue;
        for(int j = i*i; j <= n; j+=i) 
            prime[j] = false;
    }
    for(int i = 2; i < n; i++)
        if(prime[i])
            std::cout << i << " ";
}


int main() {
    Sieve(100);
    return 0;
}
