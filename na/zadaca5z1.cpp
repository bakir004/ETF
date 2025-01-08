#include <random>
#include <algorithm>
#include <utility>
#include <vector>
#include <complex>
#include <iostream>
#include <limits>
#include <stdexcept>

template <typename FunType>
bool BracketRoot(FunType f, double x0, double &a, double &b, double hinit = 1e-5, double hmax = 1e10, double lambda = 1.4) {
    if(hinit <= 0 || hmax <= 0 || lambda <= 0) throw std::domain_error("Invalid parameters");

    double h = hinit;
    a = x0;
    double f1 = f(a);
    while(std::fabs(h) < hmax) {
        b = a + h;
        double f2 = f(b);
        while(std::isnan(f2)) {
            h = h/(2*(1+lambda));
            if(std::fabs(h) <= std::fabs(a) * std::numeric_limits<double>::epsilon())
                return false;
            b = a + h;
            f2 = f(b);
        }
        if(f1*f2<=0) {
            if(a > b) std::swap(a, b);
            return true;
        }
        h = lambda * h;
        a = b;
        f1 = f2;
    }
    h = -hinit;
    a = x0;
    f1 = f(a);
    while(std::fabs(h) < hmax) {
        b = a + h;
        double f2 = f(b);
        while(std::isnan(f2)) {
            h = h/(2*(1+lambda));
            if(std::fabs(h) <= std::fabs(a) * std::numeric_limits<double>::epsilon())
                return false;
            b = a + h;
            f2 = f(b);
        }
        if(f1*f2<=0) {
            if(a > b) std::swap(a,b);
            return true;
        }
        h = lambda * h;
        a = b;
        f1 = f2;
    }
    return false;
}

enum RegulaFalsiMode {Unmodified, Illinois, Slavic, IllinoisSlavic};
template <typename FunType>
double RegulaFalsiSolve(FunType f, double a, double b, RegulaFalsiMode mode = Slavic, double eps = 1e-10, int maxiter = 100) {
    if(f(a)*f(b)>0) throw std::range_error("Root must be bracketed");
    if(eps <= 0 || maxiter <= 0) throw std::domain_error("Invalid parameters");

    auto g = [&f](double x){ return f(x)/(1+std::fabs(f(x))); };

    int iters = 0;
    double f1 = f(a);
    double f2 = f(b);
    if(mode == Slavic || mode == IllinoisSlavic) {
        f1 = g(a);
        f2 = g(b);
    }
    double c = a;
    double cold = b;
    while(std::fabs(c-cold)>eps && ++iters <= maxiter) {
        cold = c;
        c = (a*f2-b*f1)/(f2-f1);
        double f3 = f(c);
        if(mode == Slavic || mode == IllinoisSlavic) f3 = g(c);
        if(f3 == 0) return c;
        if(f1*f3<0) {
            b = a;
            f2 = f1;
        } else if(mode == Illinois || mode == IllinoisSlavic)
            f2 /= 2;
        a = c;
        f1 = f3;
    }
    if(std::fabs(c-cold)>eps)
        throw std::logic_error("Given accuracy has not achieved");
    return c;
}

double signum(double n) {
    return (n > 0) - (n < 0);
}

template <typename FunType>
double RiddersSolve(FunType f, double a, double b, double eps = 1e-10, int maxiter = 100) {
    if(f(a)*f(b)>0) throw std::range_error("Root must be bracketed");
    if(eps <= 0 || maxiter <= 0) throw std::domain_error("Invalid parameters");
    double f1 = f(a);
    double f2 = f(b);
    int iters = 0;
    while(std::fabs(b-a)>eps && ++iters<=maxiter) {
        double c = (a+b)/2;
        double f3 = f(c);
        if(f3 == 0) return c;
        double d = c + f3 * (c-a) * signum(f1-f2)/std::sqrt(f3*f3-f1*f2);
        double f4 = f(d);
        if(f4 == 0) return d;
        if(f3*f4<=0) {
            a = c;
            b = d;
            f1 = f3;
            f2 = f4;
        } else if(f1*f4<=0) {
            b = d;
            f2 = f4;
        } else {
            a = d;
            f1 = f4;
        }
    }
    if(std::fabs(b-a)>eps)
        throw std::logic_error("Given accuracy has not achieved");
    return (a+b)/2;
}

template <typename FunType1, typename FunType2>
double NewtonRaphsonSolve(FunType1 f, FunType2 fprim, double x0, double eps = 1e-10, double damping = 0, int maxiter = 100) {
    if(eps <= 0 || maxiter <= 0 || damping < 0 || damping >= 1) throw std::domain_error("Invalid parameters");
    double deltaX = std::numeric_limits<double>::infinity();
    double x = x0;
    int iters = 0;
    if(damping == 0) {
        while(std::fabs(deltaX)>eps && ++iters <= maxiter) {
            double v = f(x);
            if(std::fabs(v)<=eps) return x;
            double d = fprim(x);
            if(d == 0 || std::isnan(v))
                throw std::logic_error("Convergence has not achieved");
            deltaX = v/d;
            x -= deltaX;
        }
        if(std::fabs(deltaX)>eps)
            throw std::logic_error("Convergence has not achieved");
        return x;
    } else {
        double v = f(x);
        double d = fprim(x);
        while(std::fabs(deltaX)>eps && ++iters <= maxiter) {
            if(std::fabs(v)<=eps) return x;
            deltaX = v/d;
            double w = v;
            v = f(x-deltaX);
            d = fprim(x-deltaX);
            while(std::fabs(v)>std::fabs(w) || std::isnan(v) || d == 0) {
                deltaX *= damping;
                v = f(x-deltaX);
                d = fprim(x-deltaX);
            }
            x -= deltaX;
        }
        if(std::fabs(deltaX)>eps)
            throw std::logic_error("Convergence has not achieved");
        return x;
    }
}

typedef std::complex<double> Complex;

std::pair<Complex, bool> Laguerre(std::vector<Complex> p, double n, Complex x0, double eps = 1e-10, int maxiters = 100) {
    Complex deltaX = std::numeric_limits<double>::infinity();
    int k = 1;
    Complex x = x0;
    while(std::abs(deltaX) > eps && k < maxiters) {
        Complex f = p.at(n);
        Complex d = 0;
        Complex s = 0;
        for(int i = n-1; i>=0; i--) {
            s = s*x+2.*d;
            d = d*x+f;
            f = f*x+p.at(i);
        }
        if(std::abs(f)<=eps) return {x, true};
        Complex r = std::sqrt((n-1)*((n-1)*d*d-n*f*s));
        if(std::abs(d+r)>std::abs(d-r))
            deltaX = (n*f/(d+r));
        else
            deltaX = (n*f/(d-r));
        x = x-deltaX;
        k++;
    }
    if(std::abs(deltaX) <= eps)
        return {x,true};
    return {x,false};
}

Complex RandomComplex() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<double> dist(-10.0, 10.0);
    
    double realPart = dist(gen);
    double imagPart = dist(gen);
    
    return std::complex<double>(realPart, imagPart);
}

std::vector<Complex> PolyRoots(std::vector<Complex> coefficients, double eps = 1e-10, int maxiters = 100, int maxtrials = 10) {
    if(eps <= 0 || maxiters <= 0 || maxtrials <= 0) throw std::domain_error("Invalid parameters");
    int n = coefficients.size()-1;
    std::vector<Complex> solutions(n);
    std::vector<Complex> originalCoefficients = coefficients;
    for(int i = n; i>=1; i--) {
        int t = 1;
        bool c = false;
        Complex x = Complex();
        while(!c && t < maxtrials) {
            x = RandomComplex();
            std::pair<Complex, bool> root = Laguerre(coefficients, i, x, eps, maxiters);
            x = root.first;
            c = root.second;
            t++;
        }
        if(!c) throw std::logic_error("Convergence has not achieved");
        if(std::fabs(x.imag()) <= eps)
            x = x.real();
        // Polishing zeros
        std::pair<Complex, bool> root = Laguerre(originalCoefficients, n, x, eps, maxiters);
        if(root.second) solutions.at(i-1) = root.first;
        else solutions.at(i-1) = x;
        // End polishing zeros

        solutions.at(i-1) = x;
        Complex v = coefficients.at(i);
        for(int j = i-1; j >= 0; j--) {
            Complex w = coefficients.at(j);
            coefficients.at(j) = v;
            v = w + x*v;
        }
    }
    std::sort(solutions.begin(), solutions.end(), [](const Complex& c1, const Complex& c2) {
        if(c1.real() == c2.real())
            return c1.imag() < c2.imag();
        return c1.real() < c2.real();
    });
    return solutions;
}

std::vector<Complex> toComplexVector(std::vector<double> v) {
    std::vector<Complex> complexV(v.size());
    for(int i = 0; i < v.size(); i++)
        complexV.at(i) = v.at(i);
    return complexV;
}
std::vector<Complex> PolyRoots(std::vector<double> coefficients, double eps = 1e-10, int maxiters = 100, int maxtrials = 10) {
    if(eps <= 0 || maxiters <= 0 || maxtrials <= 0) throw std::domain_error("Invalid parameters");
    int n = coefficients.size()-1;
    std::vector<Complex> solutions(n);
    std::vector<Complex> originalCoefficients = toComplexVector(coefficients);

    int i = n;
    while(i >= 1) {
        int t = 1;
        bool c = false;
        Complex x = Complex();
        while(!c && t < maxtrials) {
            x = RandomComplex();
            std::vector<Complex> complexCoefficients = toComplexVector(coefficients);
            std::pair<Complex, bool> root = Laguerre(complexCoefficients, i, x, eps, maxiters);
            x = root.first;
            c = root.second;
            t++;
        }
        if(!c) throw std::logic_error("Convergence has not achieved");
        if(std::fabs(x.imag())<=eps) {
            x = x.real();
            // Polishing zeros
            std::pair<Complex, bool> root = Laguerre(originalCoefficients, n, x, eps, maxiters);
            if(root.second) solutions.at(i-1) = root.first;
            else solutions.at(i-1) = x;
            // End polishing zeros
            double v = coefficients.at(i);
            for(int j = i-1; j >= 0; j--) {
                double w = coefficients.at(j);
                coefficients.at(j) = v;
                v = w + x.real()*v;
            }
            i--;
        } else {
            // Polishing zeros
            std::pair<Complex, bool> root = Laguerre(originalCoefficients, n, x, eps, maxiters);
            if(root.second) {
                solutions.at(i-1) = root.first;
                solutions.at(i-2) = {root.first.real(), -root.first.imag()};
            } else {
                solutions.at(i-1) = x;
                solutions.at(i-2) = {x.real(), -x.imag()};
            }
            // End polishing zeros
            double alpha = 2*x.real();
            double beta = std::abs(x)*std::abs(x);
            double u = coefficients.at(i);
            double v = coefficients.at(i-1)+alpha*u;
            for(int j = i-2; j >= 0; j--) {
                double w = coefficients.at(j);
                coefficients.at(j) = u;
                u = v;
                v = w + alpha*v-beta*coefficients.at(j);
            }
            i -= 2;
        }
    }
    std::sort(solutions.begin(), solutions.end(), [](const Complex& c1, const Complex& c2) {
        if(c1.real() == c2.real())
            return c1.imag() < c2.imag();
        return c1.real() < c2.real();
    });
    return solutions;
}

void Testiranje() {
    auto nuleU2iMinus2 = [](double x){return x*x-4;};
    auto nuleU2iMinus2prim = [](double x){return 2*x;};
    auto nemaNula = [](double x){return x*x+1;};
    auto nemaNulaPrim = [](double x){return 2*x;};

    double a,b;
    bool ogradio;
    double nula;

    ogradio = BracketRoot(nemaNula, 0, a, b);
    if(ogradio) std::cout << "Ogradjena nula u intervalu [" << a << "," << b << "], ";
    else std::cout << "Nije uspjesno ogradjivanje\n";
    try {
        NewtonRaphsonSolve(nemaNula, nemaNulaPrim, -1);
    } catch(std::logic_error e) {
        std::cout << "GRESKA: " << e.what() << "\n";
    }

    ogradio = BracketRoot(nuleU2iMinus2, 7, a, b);
    if(ogradio) std::cout << "Ogradjena nula u intervalu [" << a << "," << b << "]\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b) << "\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b, Illinois) << "\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b, IllinoisSlavic) << "\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b, Unmodified) << "\n";
    std::cout << "Nula Ridders: " << RiddersSolve(nuleU2iMinus2, a, b) << '\n';
    ogradio = BracketRoot(nuleU2iMinus2, -7, a, b);
    if(ogradio) std::cout << "Ogradjena nula u intervalu [" << a << "," << b << "]\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b) << "\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b, Illinois) << "\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b, IllinoisSlavic) << "\n";
    std::cout << "Nula: " << RegulaFalsiSolve(nuleU2iMinus2, a, b, Unmodified) << "\n";
    std::cout << "Nula Ridders: " << RiddersSolve(nuleU2iMinus2, a, b) << '\n';

    std::cout << "Nula Newton: " << NewtonRaphsonSolve(nuleU2iMinus2, nuleU2iMinus2prim, 7) << '\n';
    std::cout << "Nula Newton: " << NewtonRaphsonSolve(nuleU2iMinus2, nuleU2iMinus2prim, -7) << '\n';


    std::vector<Complex> koefComp{-6,11,-6,1};
    std::vector<double> koefReal{-6,11,-6,1};
    std::vector<Complex> korijeni1 = PolyRoots(koefReal);
    std::vector<Complex> korijeni2 = PolyRoots(koefComp);
    std::cout << "Korijeni polinoma (x-1)(x-2)(x-3):\n";
    for(auto e : korijeni1)
        std::cout << e << " ";
    std::cout << "\nKorijeni polinoma (x-1)(x-2)(x-3):\n";
    for(auto e : korijeni2)
        std::cout << e << " ";
    std::vector<Complex> koefComp2{-1,0,0,1};
    std::vector<double> koefReal2{-1,0,0,1};
    std::vector<Complex> korijeni3 = PolyRoots(koefReal2);
    std::vector<Complex> korijeni4 = PolyRoots(koefComp2);
    std::cout << "\nKorijeni polinoma x^3-1:\n";
    for(auto e : korijeni3)
        std::cout << e << " ";
    std::cout << "\nKorijeni polinoma x^3-1:\n";
    for(auto e : korijeni4)
        std::cout << e << " ";
}

int main() {
    Testiranje();
    return 0;
}
