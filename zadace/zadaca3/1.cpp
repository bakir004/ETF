#include <cmath>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

auto KubnaInterpolacija(std::vector<std::pair<double, double>> tacke) {
    int velicina = tacke.size();
    if(velicina < 4) throw std::domain_error("Nedovoljan broj cvorova");
    std::sort(tacke.begin(), tacke.end(), [](const std::pair<double,double> &a, const std::pair<double,double> &b){
            return a.first < b.first;
            });
    for(int i = 1; i < velicina; i++)
        if(tacke[i-1].first == tacke[i].first)
            throw std::domain_error("Neispravni cvorovi");

    double x1 = tacke[0].first;
    double xn = tacke[velicina-1].first;
    return [=](double x) -> double {
        if(x < x1 || x > xn) throw std::range_error("Argument izvan opsega");
        if(x == xn) return tacke[velicina-1].second;

        int indexI = 2;
        for(int i = 0; i < velicina; i++) {
            if(tacke[i].first <= x && x < tacke[i+1].first) {
                indexI = i+1;
                break;
            }
        }
        if(indexI == 1) indexI++;
        if(indexI == velicina-1) indexI--;

        indexI--; // indexI je u intervalu 1...n; pomjeramo ga na 0...n-1
        double rezultat = 0;
        for(int k = indexI-1; k <= indexI+2; k++) {
            double xk = tacke[k].first;
            double yk = tacke[k].second;
            double proizvod = yk;
            for(int j = indexI-1; j <= indexI+2; j++) {
                if(j == k) continue;
                double xj = tacke[j].first;
                proizvod *= ((x - xj) / (xk -xj));
            }
            rezultat += proizvod;
        }

        return rezultat;
    };
}

auto KubnaInterpolacija(std::function<double(double)> f, double xmin, double xmax, double deltax) {
    if(xmin > xmax || deltax <= 0) throw std::domain_error("Nekorektni parametri");
    std::vector<std::pair<double,double>> tacke;
    double curr = xmin;
    while(curr < xmax) {
        tacke.push_back(std::make_pair(curr, f(curr)));
        curr += deltax;
    }
    if((xmax - xmin)/deltax == int((xmax - xmin)/deltax))
        tacke.push_back(std::make_pair(xmax, f(xmax)));
    try {
        return KubnaInterpolacija(tacke);
    } catch(...) {
        throw;
    }
}

int main() {
    int opcija;
    std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
    std::cin >> opcija;
    if(opcija == 1) {
        int n;
        std::vector<std::pair<double,double>> v;
        std::cout << "Unesite broj cvorova: ";
        std::cin >> n;
        std::cout << "Unesite cvorove kao parove x y: ";
        double x, y;
        for(int i = 0; i < n; i++) {
            std::cin >> x >> y;
            v.push_back(std::make_pair(x, y));
        }
        try {
            auto f = KubnaInterpolacija(v);
            while(true) {
                std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
                if(!(std::cin >> x))
                    break;
                try {
                    double vrijednost = f(x);
                    std::cout << "f(" << x << ") = " << vrijednost << "\n";
                } catch(const std::exception &err) {
                    std::cout << err.what() << "\n";
                }
            }
        } catch(const std::exception &err) {
            std::cout << err.what() << "\n";
        }
    } else if(opcija == 2) {
        double xmin, xmax, deltax;
        std::cout << "Unesite krajeve intervala i korak: ";
        std::cin >> xmin >> xmax >> deltax;
        try {
            auto postojecaFunkcija = [](double x){ return x*x + std::sin(x) + std::log(x+1); };
            auto f = KubnaInterpolacija(postojecaFunkcija,xmin,xmax,deltax);
            double x;
            while(true) {
                std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
                if(!(std::cin >> x))
                    break;
                try {
                    double vrijednost = f(x);
                    std::cout << "f(" << x << ") = " << postojecaFunkcija(x) << " fapprox(" << x << ") = " << vrijednost << "\n";
                } catch(const std::exception &err) {
                    std::cout << err.what() << "\n";
                }
            }
        } catch(const std::exception &err) {
            std::cout << err.what() << "\n";
        }
    }
    return 0;
}

