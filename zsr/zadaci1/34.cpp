#include <iostream>
#include <cmath>
#define EPSILON 0.000000000001

double korijen(double x) {
    double past = 1, curr;
    while(true) {
        curr = (past + x/past)/2;
        if(fabs(curr-past) < EPSILON * abs(x))
            break;
        past = curr;
    }
    return curr;
}

int main() {
    double x;
    std::cin >> x;
    std::cout << korijen(x) << " " << sqrt(x);
    return 0;
}