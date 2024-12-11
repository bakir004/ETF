#include <iostream>
#include <list>

int fact(int n) {
    int s = 1;
    for(int i = 1; i <= n; i++) {
        s*=i;
    }
    return s;
}

int main() {
    int n = 6;
    int s = 0;
    for(int i=0;i<n;i++) s++;
    std::cout << s;
    return 0;
    
}
