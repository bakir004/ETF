#include <algorithm>
#include <functional>
#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>


template <typename FunType>
std::pair<double, bool> RombergIntegration(FunType f, double a, double b,
        double eps = 1e-8, int nmax = 1000000, int nmin = 50) {
    if(eps < 0 || nmin < 0 || nmax < 0 || nmax < nmin)
        throw std::domain_error("Bad parameter");
    double sign = 1;
    if(a > b) {
        sign = -1;
        std::swap(a,b);
    }
    double N = 2;
    double h = (b-a)/N;
    double s = (f(a) + f(b))/2;
    double Iold = s;
    int Mmax = std::log2(nmax);
    std::vector<double> I(Mmax);
    for(int i = 0; i < Mmax; i++) {
        for(int j = 1; j <= N/2; j++)
            s += f(a+(2*j-1)*h);
        I[i] = h*s;
        double p = 4;
        for(int k = i-1; k >= 0; k--) {
            I[k] = (p*I[k+1] - I[k])/(p-1);
            p *= 4;
        }
        if(std::fabs(I[0]-Iold) <= eps && N >= nmin)
            return {sign*I[0],true};
        Iold = I[0];
        h /= 2;
        N *= 2;
    }
    return {sign*I[0],false};
}

template <typename FunType>
std::pair<double, bool> TanhSinhIntegration(FunType f, double a, double b,
        double eps = 1e-8, int nmax = 1000000, int nmin = 20, double range = 3.5) {
    if(eps < 0 || nmin < 0 || nmax < 0 || nmax < nmin || range < 0)
        throw std::domain_error("Bad parameter");
    double sign = 1;
    if(a > b) {
        sign = -1;
        std::swap(a,b);
    }
    double PI = std::atan(1)*4;
    double N = 2;
    double h = 2*range/N;
    double p = (b+a)/2;
    double q = (b-a)/2;
    double s = 0;
    double Iold = s;
    double I = 0;
    while(N < nmax) {
        for(int i = 1; i <= N/2; i++) {
            double t = -range + (2*i-1)*h;
            double u = PI*std::sinh(t)/2;
            double v = f(p+q*std::tanh(u));
            if(std::isfinite(v))
                s += q*PI*std::cosh(t)*v/(2*std::cosh(u)*std::cosh(u));
        }
        I = h*s;
        if(N >= nmin && std::fabs(I-Iold) <= eps)
            return { sign*I, true };
        Iold = I;
        N *= 2;
        h /= 2;
    }
    return { sign*I,false };
}

template <typename FunType>
std::pair<double,bool> AdaptiveAux(FunType f, double a, double b, 
        double eps, double f1, double f2, double f3, int R) {
    double c = (a+b)/2;
    double I1 = (b-a)*(f1+4*f3+f2)/6;
    double f4 = f((a+c)/2);
    if(!std::isfinite(f4)) f4 = 0;
    double f5 = f((c+b)/2);
    if(!std::isfinite(f5)) f5 = 0;
    double I2 = (b-a)*(f1+4*f4+2*f3+4*f5+f2)/12;
    if(std::fabs(I1-I2)<=eps)
        return {I2, true};
    if(R <= 0)
        return {I2, false};
    std::pair<double,bool> aux1 = AdaptiveAux(f,a,c,eps,f1,f3,f4,R-1);
    std::pair<double,bool> aux2 = AdaptiveAux(f,c,b,eps,f3,f2,f5,R-1);
    return {aux1.first + aux2.first, aux1.second && aux2.second};
}

template <typename FunType>
std::pair<double, bool> AdaptiveSimpson(FunType f, double a, double b,
        double eps, int R, int nmin) {
    double s = 0;
    double h = (b-a)/nmin;
    bool correct = true;
    for(int i = 1; i <= nmin; i++) {
        double f1 = f(a);
        if(!std::isfinite(f1)) f1 = 0;
        double f2 = f(a+h);
        if(!std::isfinite(f2)) f2 = 0;
        double f3 = f(a+h/2);
        if(!std::isfinite(f3)) f3 = 0;
        std::pair<double, bool> aux = AdaptiveAux(f,a,a+h,eps,f1,f2,f3,R);
        if(!aux.second) correct = false;
        s += aux.first;
        a += h;
    }
    return {s, correct};
}

template <typename FunType>
std::pair<double, bool> AdaptiveIntegration(FunType f, double a, double b,
        double eps = 1e-10, int maxdepth = 30, int nmin = 1) {
    if(eps < 0 || nmin < 0 || maxdepth < 0)
        throw std::domain_error("Bad parameter");
    double sign = 1;
    if(a > b) {
        sign = -1;
        std::swap(a,b);
    }
    std::pair<double, bool> res = AdaptiveSimpson(f, a, b, eps, maxdepth, nmin);
    res.first *= sign;
    return res; 
}

struct Integral {
    std::string ime;
    std::function<double(double)> f;
    double a;
    double b;
    double ocekivano;
};

void Testiranje() {
    const double PI = std::atan(1)*4;
    std::vector<Integral> integrali = {
        { [](double x) { return 1; }, 0, 1, 1.0 },                          // Constant Function
        { [](double x) { return x; }, 0, 1, 0.5 },                         // Linear Function
        { [](double x) { return x * x; }, 0, 1, 1.0 / 3.0 },               // Quadratic Function
        { [](double x) { return x * x * x; }, 0, 1, 1.0 / 4.0 },           // Cubic Function
        { [](double x) { return std::exp(x); }, 0, 1, std::exp(1) - 1 },   // Exponential Function
        { [](double x) { return 1 / x; }, 1, 2, std::log(2) },             // Reciprocal Function
        { [](double x) { return std::sqrt(x); }, 0, 1, 2.0 / 3.0 },        // Square Root Function
        { [](double x) { return std::sin(x); }, 0, PI, 2.0 },            // Sine Function
        { [](double x) { return std::cos(x); }, 0, PI, 0.0 },            // Cosine Function
        { [](double x) { return std::exp(-x * x); }, -INFINITY, INFINITY, std::sqrt(PI) } // Gaussian Function
    };
    for(auto s : integrali) {
        std::pair<double,bool> romberg = RombergIntegration(s.f, s.a, s.b);
        std::pair<double,bool> takahashi = TanhSinhIntegration(s.f, s.a, s.b);
        std::pair<double,bool> adaptive = AdaptiveIntegration(s.f, s.a, s.b);
    }
}

int main() {
    Testiranje();
    return 0;
}
