#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

typedef std::function<double(double)> funkcija;
typedef std::vector<std::pair<double, double>> vektor_parova;

funkcija KubnaInterpolacija(vektor_parova cvorovi) {
    if (cvorovi.size() < 4)
        throw std::domain_error("\nNedovoljan broj cvorova");
    std::sort(cvorovi.begin(), cvorovi.end(), [](const std::pair<double, double>& a, const std::pair<double, double>& b) {
            return a.first < b.first;
            });
    for (int i=0; i<cvorovi.size(); i++) {
        for (int j=0; j<cvorovi.size();j++) {
            if (i!=j && cvorovi.at(i).first==cvorovi.at(j).first) throw std::domain_error ("\nNeispravni cvorovi");
        }
    }
    funkcija fun = [cvorovi](double x) {
        double suma = 0;
        int indeks=1;
        for (int i = 1; i <= cvorovi.size(); i++) {
            indeks=i;
            if (indeks == 1)
                indeks++;
            if (indeks == cvorovi.size() - 2)
                indeks = cvorovi.size() - 3;
            double x_1 = cvorovi.at(0).first;
            double x_n = cvorovi.at(cvorovi.size()-1).first;
            if (x < x_1 || x > x_n)
                throw std::domain_error("Neispravni cvorovi");
            for (int k = indeks - 1; k <= indeks + 2; k++) {
                double y_k = cvorovi.at(k).second;

                for (int j = indeks - 1; j <= indeks + 2; j++) {
                    if (j != k) {
                        y_k *= (x - cvorovi.at(j).first) /
                            (cvorovi.at(k).first - cvorovi.at(j).first);
                    }
                }
                suma += y_k;
            }
        }
        return suma;
    };
    return fun;
}
funkcija KubnaInterpolacija(funkcija fun, double minimum, double maksimum,
        double razlika) {
    if (minimum > maksimum || razlika <= 0)
        throw std::domain_error("Nekorektni parametri");
    vektor_parova cvorovi_interpolacije;
    for (int i = 0;; i++) {
        std::cout<<"jhdlkj";
        if (minimum + i * razlika > maksimum) {
            std::cout<<"hej";
            break;
        }
        cvorovi_interpolacije.push_back(
                std::make_pair((minimum + i * razlika), fun(minimum + i * razlika)));
    }
    return KubnaInterpolacija(cvorovi_interpolacije);
}

int main() {
    try {

        std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
        int opcija;
        std::cin >> opcija;
        if (opcija == 1) {
            int br_cvorova;
            std::cout << "\nUnesite broj cvorova: ";
            std::cin >> br_cvorova;
            vektor_parova cvorovi;
            std::cout << "\nUnesite cvorove kao parove x y:";
            for (int i = 0; i < br_cvorova; i++) {
                double prvi;
                double drugi;
                std::cin >> prvi;
                std::cin >> drugi;
                cvorovi.push_back(std::make_pair(prvi, drugi));
            }

            double minimum = cvorovi.at(0).first;
            double maksimum = cvorovi.at(0).first;
            for (auto &par : cvorovi) {
                int pomocna = par.first;
                if (pomocna < minimum)
                    minimum = pomocna;
                if (pomocna > maksimum)
                    maksimum = pomocna;
            }

            funkcija f = KubnaInterpolacija(cvorovi);
            for (;;) {
                std::cout << "Unesite argument (ili \"kraj\" za kraj):";
                double x;
                std::cin >> x;
                if (!(std::cin))
                    break;
                std::cout << "F(" << x << ")=" << f(x);
                if (x < minimum || x > maksimum)
                    std::cout << "Argument izvan opsega";
                std::cout << "\n";
            }
        } else if (opcija == 2) {
            std::cout << "Unesite krajeve intervala i korak: ";
            double minimum, maksimum, korak;
            std::cin >> minimum >> maksimum >> korak;
            funkcija fun = [](double x) {
                return (std::pow(x, 2) + std::sin(x) + std::log(x + 1));
            };
            funkcija aproksimacija =
                KubnaInterpolacija(fun, minimum, maksimum, korak);
            for (;;) {
                std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
                double x;
                std::cin >> x;
                if (!(std::cin))
                    break;
                std::cout << "f(" << x << ")=" << fun(x) << " ";
                std::cout << "fapprox(" << x << ")=" << aproksimacija(x);
                if (x < minimum || x > maksimum)
                    std::cout << "Argument izvan opsega";
                std::cout << std::endl;
            }
        }
    } catch (std::domain_error izuzetak) {
        std::cout << izuzetak.what() << std::endl;
    }

    return 0;
}
