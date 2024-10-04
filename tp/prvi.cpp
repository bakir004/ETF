#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>

enum class TretmanNegativnih {IgnorirajZnak, Odbaci, TretirajKao0, PrijaviGresku};

bool JeKvadrat(int n) {
    int korijen = sqrt(n);
    return korijen * korijen == n;
}

void PrintVektor(std::vector<int> niz) {
    for(auto n : niz)
        std::cout << n << " ";
    std::cout << "\n";
}

int BrojKvadrata(int n) {
    if(n == 0) return 0;
    if(n < 0) throw std::domain_error("Broj mora biti nenegativan");

    if(JeKvadrat(n))
        return 1;

    int minBrojKvadrata = 5; // sigurno ce biti manji od 5
    int nKopija = n;
    int trenutniBrojKvadrata = 0;

    std::vector<int> kandidati;
    int korijen0 = std::max(int(sqrt(n/4)), 1);
    for(int i = sqrt(n); i >= korijen0; i--) {
        int k1 = n - i*i;
        kandidati.push_back(i);
        if(JeKvadrat(k1)) {
            minBrojKvadrata = std::min(minBrojKvadrata, 2);
            kandidati.push_back(sqrt(k1));
            kandidati.pop_back();
            break;
        }
        int korijen1 = std::max(int(sqrt(k1/3)), 1);
        for(int j = sqrt(k1); j >= korijen1; j--) {
            int k2 = k1 - j*j;
            kandidati.push_back(j);
            if(JeKvadrat(k2)) {
                minBrojKvadrata = std::min(minBrojKvadrata, 3);
                kandidati.push_back(sqrt(k2));
                kandidati.pop_back();
                break;
            }

            int korijen2 = std::max(int(sqrt(k2/2)), 1);
            for(int k = sqrt(k2); k >= korijen2; k--) {
                int k3 = k2 - k*k;
                kandidati.push_back(k);
                if(JeKvadrat(k3)) {
                    minBrojKvadrata = std::min(minBrojKvadrata, 4);
                    kandidati.push_back(sqrt(k3));
                    kandidati.pop_back();
                    break;
                }
                kandidati.pop_back();
            }
            kandidati.pop_back();
        }
        kandidati.pop_back();
    }
    return minBrojKvadrata;
}

std::array<std::vector<int>, 5> RazvrstajPoBrojuKvadrata(std::vector<int> niz, TretmanNegativnih tretman) {
    std::array<std::vector<int>, 5> razvrstani;

    for(int i = 0; i < niz.size(); i++) {
        int element = niz.at(i);
        if(element >= 0) {
            int brojKvadrataElementa = BrojKvadrata(element);
            razvrstani.at(brojKvadrataElementa).push_back(element);
            continue;
        }
        if(tretman == TretmanNegativnih::IgnorirajZnak) {
            int brojKvadrataElementa = BrojKvadrata(-element);
            razvrstani.at(brojKvadrataElementa).push_back(element);
        }
        else if(tretman == TretmanNegativnih::Odbaci)
            continue;
        else if(tretman == TretmanNegativnih::TretirajKao0) {
            razvrstani.at(0).push_back(element);
            continue;
        }
        else if(tretman == TretmanNegativnih::PrijaviGresku)
            throw std::domain_error("Nije predvidjeno razvrstavanje negativnih brojeva");
    }
    return razvrstani;
}

int main() {
    int n;
    std::cout << "Unesite brojeve (bilo koji ne-broj oznacava kraj): ";
    std::vector<int> niz;
    while(std::cin >> n)
        niz.push_back(n);
    try {
        std::array<std::vector<int>, 5> razvrstani = RazvrstajPoBrojuKvadrata(niz, TretmanNegativnih::PrijaviGresku);
        std::cout << "\nRezultati razvrstavanja po broju kvadrata:\n";
        for(int i = 0; i < 5; i++) {
            if(razvrstani.at(i).size() == 0) continue;
            std::cout << i << ": ";
            for(int j = 0; j < razvrstani.at(i).size(); j++)
                std::cout << razvrstani.at(i).at(j) << " ";
            std::cout << "\n";
        }
    } catch(std::domain_error err) {
        std::cout << "\nNije podrzano razvrstavanje negativnih brojeva!";
    }
    return 0;
}
