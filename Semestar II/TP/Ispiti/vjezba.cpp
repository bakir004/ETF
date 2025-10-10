#include <iostream>

class A {
    int a;
    A(): a(6) {};
    A(const B& b): a(9) {};
public:
    DajA() { return a; }
}
class B {
    int a;
    B(): a(4) {};
}

int main() {
    B b;
    A a(b);
    std::cout << a.DajA();
    return 0;
}
