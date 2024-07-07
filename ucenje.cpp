#include <iostream>

class A {
    virtual void Ispisi() {}   
};

class B : public A {
    void Ispisi() {}
};

class C : public B {
    void Ispisi() {}
};

int main() {
    A objA;
    B objB;
    C objC;
    A* a = &objA;
    B* b = &objB;
    C* c = &objC;
    std::cout << "STATICI:\n";
    std::cout << !!static_cast<A*>(a) << !!static_cast<A*>(b) << !!static_cast<A*>(c) << '\n';
    std::cout << !!static_cast<B*>(a) << !!static_cast<B*>(b) << !!static_cast<B*>(c) << '\n';
    std::cout << !!static_cast<C*>(a) << !!static_cast<C*>(b) << !!static_cast<C*>(c) << '\n';

    std::cout << "DYNAMICI:\n";
    std::cout << !!dynamic_cast<A*>(a) << !!dynamic_cast<A*>(b) << !!dynamic_cast<A*>(c) << '\n';
    std::cout << !!dynamic_cast<B*>(a) << !!dynamic_cast<B*>(b) << !!dynamic_cast<B*>(c) << '\n';
    std::cout << !!dynamic_cast<C*>(a) << !!dynamic_cast<C*>(b) << !!dynamic_cast<C*>(c) << '\n';
    return 0;
}
