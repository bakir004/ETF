#include <algorithm>
#include <cmath>
#include <ios>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <functional>
#include <vector>

typedef std::vector<double> VectorDouble;
typedef std::pair<double, bool> Par;

const double inf = std::numeric_limits<double>::infinity();

template <typename FunType>
Par Limit(FunType f, double x0, double h = 0, double eps = 1e-8, double nmax = 20) {
    if(eps <= 0 || nmax < 3 || nmax > 30) throw std::domain_error("Invalid parameters");
    std::function<double(double)> g = f;

    if(std::fabs(x0) == inf) {
        double sign = 1;
        if(x0 == -inf) sign = -1;
        g = [f, sign](double x) { return f(sign/x); };
        x0 = 0;
    }

    VectorDouble y(nmax);
    double yOld = inf;
    if(h==0) h = 0.001 * std::max(1.,std::fabs(x0));

    for(int i = 0; i < nmax; i++) {
        y.at(i) = g(x0 + h);

        double p = 2;
        for(int k = i-1; k >= 0; k--) {
            y.at(k) = (p*y.at(k+1)-y.at(k))/(p-1);
            p *= 2;
        }
        if(std::fabs(y.at(0)-yOld) < eps)
            return {y.at(0),true};
        yOld = y.at(0);
        h /= 2;
        if(std::fabs(h) < std::sqrt(eps)) break;
    }
    return {y[0],false};
}

struct LimesFunkcije {
    std::function<double(double)> funkcija;
    double tackaLimesa;
    double ocekivanaVrijednost;
};

int main() {
    std::vector<LimesFunkcije> limesi{
        {[](double x){ return std::sin(x)/x; }, 0, 1},
        {[](double x){ return std::pow((1+1/x),x); }, inf, 2.718281828},
        {[](double x){ return (std::pow(2.718281828,x)-1)/x; }, 0, 1},
        {[](double x){ return std::sin(3*x)/(2*x); }, 0, 3./2.},
        {[](double x){ return (1-std::cos(x))/(x*x); }, 0, 1./2.},
        {[](double x){ return std::pow(x,x); }, 0, 1},
        {[](double x){ return std::log(x)/x; }, inf, 0},
        {[](double x){ return std::log(x+1)/x; }, 0, 1},
    };
    std::cout.precision(10);
    for(auto s : limesi) {
        Par limes = Limit(s.funkcija, s.tackaLimesa);
        std::cout << "Tacnost dostignuta: " << std::boolalpha << limes.second
            << ", dobijena vrijednost: " << limes.first
            << ", ocekivana vrijednost: " << s.ocekivanaVrijednost << "\n";
    }
    return 0;
}
