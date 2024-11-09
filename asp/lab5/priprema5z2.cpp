#include<iostream>

using namespace std;
int nzd(int x, int y) {
    if(y == 0) return x;
    return nzd(y, x % y);
}

int main() {
    std::cout << nzd(10, 15) << std::endl;
    return 0;
}
