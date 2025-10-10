#include <functional>
#include<iostream>
#include <stdexcept>

void IspisiKvadrat(const int& x) {
    std::cout << x*x;
}

void IzmnoziElementeSaDva(std::vector<int>& v) {
    for(int i = 0; i < v.size(); i++)
        v[i] = v[i]*2;
}

int NajveciElement(const std::vector<int>& v) {
    // v.push_back(2); greska jer je konstanta referenca a ja hocu da je mijenjam
    int najveci = v[0];
    for(int i = 1; i < v.size(); i++)
        if(v[i] > najveci)
            najveci = v[i];

    return najveci;
}

std::vector<int> SaberiDvaNiza(const std::vector<int>& v1, const std::vector<int>& v2) {
    if(v1.size() != v2.size()) {
        throw std::domain_error("Nesaglasne dimenzije vektora");
    }

    std::vector<int> zbir(v1.size());
    for(int i = 0; i < zbir.size(); i++) {
        zbir[i] = v1[i] + v2[i];
    }
    return zbir;
}

int& DajReferencuNaNajveciElement(std::vector<int>& v) {
    int indexNajveceg = 0;
    for(int i = 1; i < v.size(); i++) {
        if(v[i] > v[indexNajveceg]) {
            indexNajveceg = i;
        }
    }

    std::cout << v[indexNajveceg] << '\n';

    int& refNaNajveci = v[indexNajveceg];
    return refNaNajveci;
}

int main() {
    std::vector<int> brojevi1{1,2,32,4,5};
    DajReferencuNaNajveciElement(brojevi1) = 9; // niz ce postati 1,2,9,4,5
    for(int i = 0; i < brojevi1.size(); i++) {
        std::cout << brojevi1[i] << " ";
    }
    // std::vector<int> brojevi2{5,0,-36,1,8};
    // std::vector<int> rezultat = SaberiDvaNiza(brojevi1, brojevi2);
    // std::vector<int> rezultat2 = SaberiDvaNiza({1,2,3}, {3,2,1});
    // for(int i = 0; i < rezultat.size(); i++) {
    //     std::cout << rezultat[i] << " ";
    // }

    // int a = 5;
    // IspisiKvadrat(5); radi ako funkcija prima const referencu
    // std::cout << NajveciElement(brojevi);

    // std::vector<int> brojevi{1,2,32,4,5};
    // IzmnoziElementeSaDva(brojevi);
    // for(int i = 0; i < brojevi.size(); i++)
    //     std::cout << brojevi[i] << " ";

    // int a = 5;
    // Kvadriraj(a);
    // Kvadriraj(3); // ovo ne moze, funkcija ocekuje l-vrijednost
    // std::cout << a; // ispisuje 25
    return 0;
}
