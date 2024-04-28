#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

template <typename T>
int** AlocirajFragmentirano(const std::vector<T> &v, int koliko_puta) {
    if(v.size() == 0) throw "Prazan vektor!";
    if(koliko_puta <= 0) throw std::domain_error("Neispravan broj ponavljanja!");
    int **matrica = nullptr;
    int brojRedova = v.size()*koliko_puta;
    int uspjesnoAlocirani = 0;
    try {
        matrica = new int*[brojRedova]{};
        int maxElement = *std::max_element(v.begin(), v.end());
        for(int i = 0; i < brojRedova; i++, uspjesnoAlocirani++) {
            int velicinaReda = v[i%v.size()];
            if(velicinaReda <= 0) throw std::domain_error("Neispravan vektor!");
            matrica[i] = new int[velicinaReda];
            for(int j = 0; j < velicinaReda; j++)
                matrica[i][j] = maxElement - velicinaReda + j + 1;
        }
    } catch(...) {
        for(int i = 0; i < uspjesnoAlocirani; i++)
            delete[] matrica[i];
        delete[] matrica;
        throw;
    }
    return matrica;
}

template <typename T>
int** AlocirajKontinualno(const std::vector<T> &v, int koliko_puta) {
    if(v.size() == 0) throw "Prazan vektor!";
    if(koliko_puta <= 0) throw std::domain_error("Neispravan broj ponavljanja!");
    int **matrica = nullptr;
    int brojRedova = v.size()*koliko_puta;
    int uspjesnoAlocirani = 0, sirina = 0;
    for(int i = 0; i < v.size(); i++) {
        if(v[i] <= 0) throw std::domain_error("Neispravan vektor!");
    }
    sirina *= koliko_puta;
    try {
        matrica = new int*[brojRedova]{};
        matrica[0] = new int[sirina];
        for(int i = 1; i < brojRedova; i++) {
            int velicinaReda = v[(i-1)%v.size()];
            matrica[i] = matrica[i-1] + velicinaReda;
        }
        for(int i = 0; i < brojRedova; i++) {
            int velicinaReda = v[i%v.size()];
            for(int j = 0; j < velicinaReda; j++)
                matrica[i][j] = velicinaReda - j;
        }
    } catch(...) {
        delete[] matrica[0];
        delete[] matrica;
        throw;
    }
    return matrica;
}

template <typename T>
int** KreirajPoUvrnutomPravilu(std::vector<T> &v, int koliko_puta, bool fragmentirano = true) {
    if(v.size() == 0) throw "Prazan vektor!";
    int period = 2;
    while(period < v.size()) {
        bool periodican = true;
        for(int i = 0; i < v.size()-period; i++) {
            if(v[i] != v[i+period]) { 
                periodican = false;
                break;
            }
        }
        if(periodican) break;
        period++;
    }
    if(period != v.size()) {
        v.resize(period);
    } else {
        int maxElement = *std::max_element(v.begin(), v.end());
        std::transform(v.begin(), v.end(), v.begin(), [maxElement](int x){ return maxElement + 1 - x; });
    }
    int** matrica = nullptr;
    try {
        if(fragmentirano)
            matrica = AlocirajFragmentirano(v, koliko_puta);
        else 
            matrica = AlocirajKontinualno(v, koliko_puta);
    } catch(...) {
        throw;
    }
    return matrica;
}

int main() {
    int n, f;
    std::cout << "Unesite broj elemenata: ";
    std::cin >> n;
    std::vector<int> v(n);
    std::cout << "Unesite elemente vektora: ";
    for(int i = 0; i < n; i++)
        std::cin >> v[i];
    std::cout << "Koliko puta pa f: ";
    std::cin >> n >> f;
    try {
        int** matrica = KreirajPoUvrnutomPravilu(v, n, f);
        for(int i = 0; i < v.size()*n; i++) {
            for(int j = 0; j < v[i%v.size()]; j++)
                std::cout << std::left << std::setw(3) << matrica[i][j];
            std::cout << "\n";
        }
        for(int i = 0; i < v.size()*n; i++)
            delete[] matrica[i];
        delete[] matrica;
    } catch(...) {
        std::cout << "ERR";
    }
    return 0;
}