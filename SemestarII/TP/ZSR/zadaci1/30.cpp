#include <iostream>
#include <cmath>

bool isPrime(int n) {
    for(int i = 2; i <= sqrt(n); i++)
        if(n%i==0) return false;
    return true;
}

int main() {
    int n, curr = 2;
    std::cin >> n;
    while(n) {
        if(isPrime(curr))
            n--;
        curr++;
    }
    std::cout << curr-1;
    return 0;
}