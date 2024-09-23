#include <iostream>
class Bazna {
    int p;
public:
    Bazna() : p(0) {}
    Bazna(int x): p(x) {}
}; 
class Izvedena : public Bazna {
    int q;
public:
    Izvedena(int p, int q) : Bazna(p), q(q) {}
};
int main() {
    Bazna b; Izvedena i(3, 5);
    return 0;
}
