#include <iostream>
#include <vector>

class A {

public:
    ~A() { std::cout << "DESC\n"; }
    int operator+(int x) {
        std::cout << "Clanica\n";
        return 1;
    }
    friend int operator+(A klasa, int x);
};

int operator+(A klasa, int x) {
    std::cout << "Friend\n";
    return 2;
}

int main() {
    A a;
    auto b = std::move(a);
    std::cout << "OK";
    int d = 0;
    d -=- 1;
    std::cout << d;
    return 0;
}
