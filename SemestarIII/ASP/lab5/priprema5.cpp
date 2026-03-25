#include<iostream>

using namespace std;

int fib2_0(int n, int a = 0, int b = 1) {
    if(n == 0) return a;
    if(n == 1) return b;
    return fib2_0(n - 1, b, a + b);
}

int nzd(int x, int y) {
    if(y == 0) return x;
    return nzd(y, x % y);
}
int main() {
    for(int i = 1; i < 40; i++)
        std::cout << fib2_0(i) << " ";
    
    std::cout << std::endl;

    for(int i = 1; i < 100; i++) {
        std::cout << i << ", " << 100-i << ": " << nzd(i, 100-i) << "\n";
    }
    return 0;
}
