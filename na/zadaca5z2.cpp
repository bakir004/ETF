#include <cmath>
#include <functional>
#include<iostream>
#include <stdexcept>

template <typename FunType>
double FindMinimum(FunType f, double x0, double eps = 1e-8, double hinit = 1e-5, double hmax = 1e10, double lambda = 1.4) {
    if(eps <= 0 || hinit <= 0 || hmax <= 0 || lambda <= 0) throw std::domain_error("Invalid parameters");
    double a,b;
    double h = hinit;
    double xiMinus2 = x0;
    double xiMinus1 = x0;
    double xi = x0;
    if(f(x0+h)>f(x0)&&f(x0-h)>f(x0)) {
        a = x0-h;
        b = x0+h;
    }
    if(f(x0+h)>f(x0)) h = -h;
    if(f(x0+h)<f(x0)) {
        xiMinus2 = x0;
        xiMinus1 = xiMinus2 + h;
        h *= lambda;
        xi = xiMinus1 + h;
        h *= lambda;
        while(!(f(xiMinus1) < f(xiMinus2) && f(xiMinus1) < f(xi))) {
            double novi = xi + h;
            xiMinus2 = xiMinus1;
            xiMinus1 = xi;
            xi = novi;
            h *= lambda;
            if(std::fabs(h) > hmax) throw std::logic_error("Minimum has not found");
        }
    }
    a = xiMinus2;
    b = xi;
    if(a > b) std::swap(a,b);

    double kStar = (std::sqrt(5)-1)/2;
    double c = b-kStar*(b-a); double d = a+kStar*(b-a);
    double u = f(c);
    double v = f(d);
    while(std::fabs(b-a) > eps) {
        if(u < v) {
            b = d;
            d = c;
            c = a+kStar*(c-a);
            v = u;
            u = f(c);
        } else {
            a = c;
            c = d;
            d = b-kStar*(b-d);
            u = v;
            v = f(d);
        }
    }
    return (a+b)/2;
}

struct FunctionInfo {
    std::string functionName;             
    std::function<double(double)> function;
    double expectedMinimum;                 
};

void Testiranje() {
    std::vector<FunctionInfo> functions = {
        {
            "f(x) = (x - 1)^2", 
            [](double x) { return (x - 1) * (x - 1); }, 
            1.0
        },
        {
            "f(x) = (x + 2)^2", 
            [](double x) { return (x + 2) * (x + 2); }, 
            -2.0
        },
        {
            "f(x) = x^2 + 3", 
            [](double x) { return x * x + 3; }, 
            0.0
        },
        {
            "f(x) = (x - 4)^2 + 5", 
            [](double x) { return (x - 4) * (x - 4) + 5; }, 
            4.0
        },
        {
            "f(x) = x^2 - 2x + 1", 
            [](double x) { return x * x - 2 * x + 1; }, 
            1.0
        }
    };
    std::cout.precision(10);
    for(auto f : functions) {
        std::cout << f.functionName << ":\n";
        std::cout << "  Ocekivani minimum: " << f.expectedMinimum << "\n";
        std::cout << "  Pronadjeni minimum: " << FindMinimum(f.function, 5) << "\n";
    }
}

int main() {
    Testiranje();
    return 0;
}
