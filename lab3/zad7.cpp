#include <iostream>
#include <stdexcept>
#include <vector>
#include <iomanip>

typedef std::vector<std::vector<int> > Matrica;

int Sirina(int n) {
    int sirina = 0;
    if(n < 0) {
        sirina++;
        n *= -1;
    }
    while(n > 0) {
        n /= 10;
        sirina++;
    }
    return sirina;
}

int NajvecaSirina(Matrica &matrica) {
    int maxSirina = 0;
    for(int i = 0; i < matrica.size(); i++) 
        for(int j = 0; j < matrica[i].size(); j++) 
            maxSirina = std::max(Sirina(matrica[i][j]), maxSirina);
    return maxSirina;
}

Matrica PascalovTrougao(int n) {
    Matrica matrica; 
    for(int i = 1; i <= n; i++) {
        std::vector<int> red(i,0);
        red[0] = 1;
        red[red.size()-1] = 1;
        for(int j = 1; j < red.size() - 1; j++) 
            red[j] = matrica[i-2][j] + matrica[i-2][j-1];

        matrica.push_back(red);
    }
    return matrica;
}

int main() {
    int n;
    std::cout << "Unesite broj redova: ";
    std::cin >> n;
    if(n < 0) 
        throw std::domain_error("Broj redova ne smije biti negativan.");
    

    Matrica matrica = PascalovTrougao(n);
    int maxSirina = NajvecaSirina(matrica);
    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) 
            std::cout << std::setw(maxSirina) << matrica[i][j] << " ";
        std::cout << "\n";
    }
}

