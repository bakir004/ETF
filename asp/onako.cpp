#include <iostream>
#include <vector>

int F(std::vector<int> &v, int a, int& b) {
    if(a+b>200)
        return a+b;
    b+=a;
    v.push_back(F(v,a+b,b)+a);
    return b;
}

int main() {
    std::vector<int> v;
    int b = 5;
    F(v,13,b);
    for(auto e : v)
        std::cout << e << " ";
    return 0;
}
