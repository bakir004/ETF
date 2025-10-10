#include <iostream>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <vector>

typedef std::vector<double> VectorDouble;

class ChebyshevApproximation {
    int m, n;
    VectorDouble c;
    const double PI = std::atan(1)*4;
    double xmin, xmax;
    std::function<double(double)> f;
    template <typename FunType>
    ChebyshevApproximation(FunType f, double xmin, double xmax, int n, int m, VectorDouble c) {
        this->f = f;
        this->m = m;
        this->n = n;
        this->xmin = xmin;
        this->xmax = xmax;
        this->c = c;
    }
public:
    template <typename FunType>
    ChebyshevApproximation(FunType f, double xmin, double xmax, int n) {
        if(xmin > xmax || n < 1) throw std::domain_error("Bad parameters");
        this->f = f;
        this->m = n;
        this->n = n;
        this->xmin = xmin;
        this->xmax = xmax;

        c = VectorDouble(n+1);
        VectorDouble w = VectorDouble(n+2);
        VectorDouble v = VectorDouble(n+1);
        for(int i = 0; i <= n+1; i++)
            w.at(i) = std::cos(PI*i/(2*n+2));
        for(int i = 0; i <= n/2; i++)
            v.at(i) = f((xmin+xmax+(xmax-xmin)*w.at(2*i+1))/2);
        for(int i = n/2+1; i <= n; i++)
            v.at(i) = f((xmin+xmax-(xmax-xmin)*w.at(2*n+1-2*i))/2);
        for(int k = 0; k <= n; k++) {
            double s = 0;
            for(int i = 0; i <= n; i++) {
                int p = (k*(2*i+1))%(4*n+4);
                if(p>2*n+2)
                    p = 4*n+4-p;
                if(p>n+1)
                    s-=v.at(i)*w.at(2*n+2-p);
                else
                    s+=v.at(i)*w.at(p);
            }
            c.at(k)=2*s/(n+1);
        }
    }
    void set_m(int m) {
        if(m <= 1 || m > n) throw std::domain_error("Bad order");
        this->m = m;
    }
    void trunc(double eps) {
        if(eps < 0) throw std::domain_error("Bad tolerance");
        int count = 0;
        for(int i = 0; i < c.size(); i++)
            if(std::fabs(c.at(i)) < eps)
                count++;
        if(count == c.size())
            throw std::domain_error("Bad tolerance");
        for(int i = c.size()-1; i >= 0; i--) {
            if(std::fabs(c.at(i)) >= eps) {
                m = i;
                break;
            }
        }
    }
    double operator()(double x) const {
        if(x < xmin || x > xmax) throw std::domain_error("Bad argument");
        double t = (2*x-xmin-xmax)/(xmax-xmin);
        double p = 0;
        double q = c.at(m);
        for(int k = m-1; k >= 1; k--) {
            double r = 2*t*q-p+c.at(k);
            p = q;
            q = r;
        }
        return t*q-p+c.at(0)/2;
    }
    double derivative(double x) const {
        if(x < xmin || x > xmax) throw std::domain_error("Bad argument");
        double t = (2*x-xmin-xmax)/(xmax-xmin);
        double p = 1;
        double q = 4*t;
        double s = c.at(1)+q*c.at(2);
        for(int k = 3; k <= m; k++) {
            double r = k*(2*t*q/(k-1)-p/(k-2));
            s += c.at(k)*r;
            p = q;
            q = r;
        }
        return 2*s/(xmax-xmin);
    }
    ChebyshevApproximation derivative() const {
        VectorDouble cprimc = VectorDouble(m+1);
        double mi = 4/(xmax-xmin);
        cprimc.at(m-1) = mi * m * c.at(m);
        cprimc.at(m-2) = mi * (m-1) * c.at(m-1);
        for(int k = m-3; k >= 0; k--)
            cprimc.at(k) = cprimc.at(k+2) + mi*(k+1)*c.at(k+1);
        return ChebyshevApproximation(f, xmin, xmax, n, m, cprimc);
    }
    ChebyshevApproximation antiderivative() const {
        VectorDouble cant(m + 1); 
        double mi = (xmax - xmin) / 4;

        cant.at(0) = 0;
        for (int k = 1; k < m; ++k)
            cant.at(k) = mi * (c.at(k-1)-c.at(k+1)) / k;

        cant.at(m) = mi * c.at(m-1) / m ;
        cant.push_back(mi * c.at(m)/(m+1));
        return ChebyshevApproximation(f, xmin, xmax, n, m + 1, cant);
    }
    double integrate(double a, double b) const {
        if (a < xmin || b > xmax || a > b)
            throw std::domain_error("Bad interval");
        return antiderivative()(b) - antiderivative()(a);
    }
    double integrate() const {
        return integrate(xmin, xmax);
    }
};

struct Funkcija {
    std::string naziv; 
    std::function<double(double)> funkcija;
    std::function<double(double)> izvod;
    std::function<double(double)> integral;
};

int main() {
    std::vector<Funkcija> funkcije = {
        {
            "Kvadratna funkcija (x^2)", 
            [](double x) { return x * x; },               
            [](double x) { return 2 * x; },              
            [](double x) { return (x * x * x) / 3; }    
        },
        {
            "Kubna funkcija (x^3)", 
            [](double x) { return x * x * x; },        
            [](double x) { return 3 * x * x; },        
            [](double x) { return (x * x * x * x) / 4; }
        },
        {
            "Sinusna funkcija (sin(x))", 
            [](double x) { return std::sin(x); },        
            [](double x) { return std::cos(x); },       
            [](double x) { return -std::cos(x); }      
        },
        {
            "Kosinusna funkcija (cos(x))", 
            [](double x) { return std::cos(x); },     
            [](double x) { return -std::sin(x); },   
            [](double x) { return std::sin(x); }    
        },
        {
            "Eksponencijalna funkcija (e^x)", 
            [](double x) { return std::exp(x); },  
            [](double x) { return std::exp(x); }, 
            [](double x) { return std::exp(x); } 
        },
    };

    for (Funkcija funkcija : funkcije) {
        double x = 3.0; 
        ChebyshevApproximation f(funkcija.funkcija, -7, 7, 20);
        ChebyshevApproximation izvod = f.derivative(); 
        ChebyshevApproximation integral = f.antiderivative(); 
        std::cout << "Naziv funkcije: " << funkcija.naziv << "\n";
        std::cout << "  Vrijednost funkcije: " << funkcija.funkcija(x) << ", approx: " << f(x) << "\n";
        std::cout << "  Vrijednost izvoda: " << funkcija.izvod(x) << ", approx: " << izvod(x) << "\n";
        std::cout << "  Vrijednost integrala: " << funkcija.integral(x)-funkcija.integral(0) << ", approx: " << integral(x)-integral(0) << "\n";
    }

    return 0;
}
