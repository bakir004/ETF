#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>

enum class TretmanNegativnih {IgnorirajZnak, Odbaci, TretirajKao0, PrijaviGresku};

int DFS(int n, int dubina) {
    if(n == 0) return dubina;
    if(dubina > 4) return 999;

    int minBrojKvadrata = 5;
    for(int i = sqrt(n); i >= sqrt(n/(4-dubina)); i--)
        minBrojKvadrata = std::min(minBrojKvadrata, DFS(n - i*i, dubina+1));
    
    return minBrojKvadrata;
}

int BrojKvadrata(int n) {
    if(n == 0) return 0;
    if(n < 0) throw std::domain_error("Broj mora biti nenegativan");

    return DFS(n, 0);
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
