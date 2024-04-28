#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include <cmath>

int main() {
    std::list<double> a{1, 4, 2, 5, 3}; ;
    auto f = [](double (*g)(int)) { return g(1); };
    std::cout << f([](int x){ return (double)x; });
    double *p = std::find(a.begin(), a.end(), 2); std::cout << *p;
    // for(int x : q) std::cout << x << " "; return 0;
}
