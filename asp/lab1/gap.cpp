#include <iostream>
#include <vector>
#include <bitset>
#define UPPER_BOUND 1000001

std::bitset<UPPER_BOUND> prime;

void Sieve(int n, std::vector<int>& primes) {
    prime.set();  // Set all bits to true
    prime[0] = prime[1] = false;
    
    for (int i = 2; i <= n; i++) {
        if (prime[i]) {
            primes.push_back(i);
            for (int j = i * 2; j <= n; j += i) {
                prime[j] = false;
            }
        }
    }
}

int main() {
    int n = 1000000;

    std::vector<int> primes;
    
    // Generate all primes up to n
    Sieve(n, primes);
    
    // Calculate the total gap and the average gap
    long long total_gap = 0;
    for (size_t i = 1; i < primes.size(); i++) {
        total_gap += primes[i] - primes[i - 1];
    }
    
    double average_gap = static_cast<double>(total_gap) / (primes.size() - 1);
    
    std::cout << "Average gap between primes: " << average_gap << "\n";
    return 0;
}
