#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

void CitajFajl() {
    std::ifstream ulazniTok("ulaz.txt");

    std::vector<std::vector<int>> matrica;
    matrica.push_back({});
    int indeks = 0;
    while(!ulazniTok.eof()) {
        int broj;
        ulazniTok >> broj;
        if(ulazniTok.eof()) break;

        if(ulazniTok.peek() == '\n') {
            matrica[indeks].push_back(broj);        
            matrica.push_back({});
            indeks++;
            continue;
        }
        if(!ulazniTok) throw;
        char c;
        ulazniTok >> c;
        if(c != ',') throw;
        matrica[indeks].push_back(broj);
    }
    matrica.erase(matrica.begin() + matrica.size() - 1);

    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) {
            std::cout << matrica[i][j] << " ";
        }
        std::cout << '\n';
    }

    ulazniTok.close();
    std::ofstream izlazniTok("izlaz.txt");
    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) {
            izlazniTok << matrica[i][j] << " ";
        }
        izlazniTok << '\n';
    }
    izlazniTok.close();
}

void PisiICitajBinarno() {
    std::ofstream izlazniTok("izlazbin.dat", std::ios::binary);

    std::vector<int> niz{1,2,3,4,5,6,7,8,9};

    for(int i = 0; i < niz.size(); i++) {
        izlazniTok.write(reinterpret_cast<char*>(&niz[i]), sizeof niz[i]);
    }
    izlazniTok.close();

    std::ifstream ulazniTok("izlazbin.dat", std::ios::binary);
    ulazniTok.seekg(0, std::ios::end);
    int velicinaNiza = ulazniTok.tellg() / sizeof(int);
    std::vector<int> procitano(velicinaNiza);

    ulazniTok.seekg(0, std::ios::beg);

    for(int i = 0; i < procitano.size(); i++) {
        ulazniTok.read(reinterpret_cast<char*>(&procitano[i]), sizeof procitano[i]);
    }
    for(int i = 0; i < procitano.size(); i++) {
        std::cout << procitano[i] << ",";
    }
}

int main() {
    PisiICitajBinarno();
    return 0;
}
