#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>

enum class TretmanNegativnih {IgnorirajZnak, Odbaci, TretirajKao0, PrijaviGresku};

int BrojKvadrata(int n) {
    if(n == 0) return 0;
    if(n < 0) throw std::domain_error("Broj mora biti nenegativan");

    int korijen = sqrt(n);
    if(korijen * korijen == n)
        return 1;

    int minBrojKvadrata = 5; // sigurno ce biti manji od 5
    int nKopija = n;
    int trenutniBrojKvadrata = 0;

    for(int i = sqrt(n); i >= sqrt(nKopija/4); i--) {
        n = nKopija - i*i; // probavamo sve potpune kvadrate manje od originalnog 'n'
        trenutniBrojKvadrata++;
        for(int j = i; j >= 1; j--) {
            // probavamo sve potpune kvadrate manje od kusura od prethodnog koraka
            if(n - j*j >= 0) { 
                n -= j*j;
                trenutniBrojKvadrata++;
                // ako smo uspjeli, n (kusur) ce biti 0
                if(trenutniBrojKvadrata <= 4 && n == 0) { // ne zelimo ici dalje putem koji ima vise od 4 kvadrata
                    minBrojKvadrata = std::min(minBrojKvadrata, trenutniBrojKvadrata);
                    break;
                } // ako nismo uspjeli, oduzmi opet jos jednom kvadrat is prethodnog koraka
                else if(n - j*j >= 0) j++;
            }
        }
        trenutniBrojKvadrata = 0;
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
    std::cout << "Unesite brojeve (bilo koji ne-broj oznacava kraj): ";
    int n;
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
