#include <exception>
#include <iomanip>
#include <iostream>
#include <complex>
#include <stdexcept>
#include <vector>

template <typename T>
void KreirajTabeluSabiranja(T **&tabela, std::vector<T> v1, std::vector<T> v2) {
    try {
        tabela = new T*[v1.size()]{};
        tabela[0] = new T[v1.size() * v2.size()];
        for(int i = 1; i <= v1.size(); i++)
            tabela[i] = tabela[i-1] + v2.size();

        for(int i = 0; i < v1.size(); i++)
            for(int j = 0; j < v2.size(); j++)
                tabela[i][j] = v1[i] + v2[j];
    } catch(...) {
        delete[] tabela;
        throw std::domain_error("Kreiranje tabele nije uspjelo");
    }
}

template <typename T>
void UnistiTabelu(T **tabela) {
    delete[] tabela[0];
    delete[] tabela;
}

int main() {
    std::complex<double> **matrica;
    std::vector<std::complex<double>> v1{{1,2},{3,4},{5,6},{7,8}};
    std::vector<std::complex<double>> v2{{1,2},{3,4},{4,5}};
    try {
        KreirajTabeluSabiranja(matrica, v1, v2);
        for(int i = 0; i < v1.size(); i++) {
            for(int j = 0; j < v2.size(); j++)
                std::cout << std::setw(7) << matrica[i][j] << " ";
            std::cout << std::endl;
        }
        UnistiTabelu(matrica);
    } catch(std::exception &err) {
        std::cout << err.what() << "\n";
    }
    return 0;
}