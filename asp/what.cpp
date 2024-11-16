#include <iostream>

int digitSum(int n) {
    int sum = 0;
    while(n > 0) {
        sum+=n%10;
        n /= 10;
    }
    return sum;
}

int main() {
    int sum = 0;
    for(int i = 1; i < 10000; i++)
        if(digitSum(i) == 9)
            std::cout << i << " ";
    return 0;
}
