#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

class Vector {
    std::vector<double> elementi;
public:
    explicit Vector(int n) { 
        if(n <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<double>(n, 0); 
    }
    Vector(std::initializer_list<double> l) {
        if(l.size() <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<double>(l);
    }
    /*bez templatea ovo ne radi*/
    template <typename T>
    friend double VectorNorm(const Vector& v);
};

template <typename T>
double VectorNorm(const T& v) {
    std::cout << "genericka";
    return 0;
}

/*double VectorNorm(const Vector &v) {*/
/*    std::cout << "tipicna";*/
/*    return 0;*/
/*}*/

int main() {
    Vector x({1, 2, 3, 4, 5});
    std::cout << VectorNorm(x);
}
