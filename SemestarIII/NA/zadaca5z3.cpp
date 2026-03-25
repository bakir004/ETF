#include<iostream>
#include<cmath>
#include<stdexcept>
#include<vector>

typedef std::pair<double,double> Pair;
typedef std::vector<Pair> VectorPair;

const double PI = std::atan(1)*4;

template <typename FunType>
double RK4Step(FunType f, double x0, double y0, double h) {
    double x = x0;
    double y = y0;
    double K1 = f(x,y);
    double K2 = f(x+h/2, y + h*K1/2);
    double K3 = f(x+h/2, y+h*K2/2);
    double K4 = f(x+h,y+h*K3);
    return y + h*(K1+2*K2+2*K3+K4)/6;
}

template <typename FunType>
VectorPair RK4Integrator(FunType f, double x0,
                         double y0, double xmax, double h, 
                         double eps = 1e-8, bool adaptive = false) {
    if(eps <= 0) throw std::domain_error("Invalid parameters");

    VectorPair sols;
    double x = x0;
    double y = y0;
    if(h > 0 && xmax < x0 || h < 0 && xmax > x0) {
        sols.push_back({x,y}); 
        return sols;
    }
    if(!adaptive) {
        double delta = h/100;
        while((h > 0 && x <= xmax + delta) || (h < 0 && x >= xmax + delta)) {
            sols.push_back({x,y});
            double K1 = f(x,y);
            double K2 = f(x+h/2, y + h*K1/2);
            double K3 = f(x+h/2, y+h*K2/2);
            double K4 = f(x+h,y+h*K3);
            y += h*(K1+2*K2+2*K3+K4)/6;
            x += h;
        }
        return sols;
    }
    double delta = 0;
    int iters = 0;
    while((h > 0 && x <= xmax) || (h < 0 && x >= xmax)) {
        double u = RK4Step(f, x, y, h/2);
        double v = RK4Step(f, x+h/2, u, h/2);
        double w = RK4Step(f, x, y, h);
        delta = std::fabs((w-v)/h);
        if(delta <= eps) {
        if(h > 0 && x + h > xmax || h < 0 && x + h < xmax) {
                h = xmax-x;
                continue;
            }
            x += h;
            y = v;
            sols.push_back({x,y});
        }
        /*std::cout << "x: " << x << "\n";*/
        if(++iters > 100) break;
        h *= std::min(5., 0.9*std::pow(eps/delta, 0.25));
    }

    return sols;
}

void Testiranje() {
    const double eps = 1e-10;

    auto res1 = RK4Integrator([](double x, double y) { return 2 * x + y + 1; },
                              0, 1, 1, 0.1, eps, true);
    auto exact1 = [](double x) { return -3 - 2 * x + 4 * std::exp(x); };
    for (const auto& [x, y] : res1) {
        if (std::fabs(y - exact1(x)) > eps) {
            std::cout << "Test 1 pao kod x = " << x << std::endl;
            return;
        }
    }

    auto res2 = RK4Integrator([](double x, double y) { return y; },
                              0, 1, 1, 0.1, eps, true);
    auto exact2 = [](double x) { return std::exp(x); };
    for (const auto& [x, y] : res2) {
        if (std::fabs(y - exact2(x)) > eps) {
            std::cout << "Test 2 pao kod x = " << x << std::endl;
            return;
        }
    }

    auto res3 = RK4Integrator([](double x, double y) { return std::cos(x); },
                              0, 0, PI, 0.1, eps, true);
    auto exact3 = [](double x) { return std::sin(x); };
    for (const auto& [x, y] : res3) {
        if (std::fabs(y - exact3(x)) > eps) {
            std::cout << "Test 3 pao kod x = " << x << std::endl;
            return;
        }
    }

    auto res4 = RK4Integrator([](double x, double y) { return y * (1 - y); },
                              0, 0.1, 10, 0.1, eps, true);
    auto exact4 = [](double x) { return 1 / (1 + 9 * std::exp(-x)); };  
    for (const auto& [x, y] : res4) {
        if (std::fabs(y - exact4(x)) > eps) {
            std::cout << "Test 4 pao kod x = " << x << std::endl;
            return;
        }
    }
    std::cout << "Svi testovi prosli!" << std::endl;
}

int main() {
    Testiranje();
    return 0;
}
