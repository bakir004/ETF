#include <iostream>
#include <deque>
#include <stdexcept>
#include <vector>

std::deque<double> Funkcija(std::vector<std::vector<double>> &matrica) {
    if(matrica.size() == 0) throw std::domain_error("Parametar nema formu matrice");
    int sirina = matrica.at(0).size();
    for(auto v : matrica)
        if(v.size() != sirina) 
            throw std::domain_error("Parametar nema formu matrice");

    std::deque<double> dek(matrica.size());
    for(int j = 0; j < matrica.size(); j++) {
        auto &v = matrica.at(j);
        for(int i = 0; i < v.size()/2; i++) {
            auto temp  = v[i];
            v[i] = v[v.size()-i-1];
            v[v.size()-i-1] = temp;
            dek[j] += v[i] + v[v.size()-i-1];
        }
    }
    return dek;
}

int main() {
    std::vector<std::vector<double>> matrica{{1,2,3,4},{5,7,8},{9,10,11,12}};
    std::deque<double> dek = Funkcija(matrica);
    for(auto v : matrica) {
        for(auto e : v)
            std::cout << e << " ";
        std::cout << '\n';
    }
    std::cout << "\n";
    for(auto e : dek)
        std::cout << e << " ";

    return 0;
}
