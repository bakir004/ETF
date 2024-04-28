#include <iostream>
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

Matrica KroneckerovProizvod(std::vector<int> &a, std::vector<int> &b) {
    int m = a.size(), n = b.size();
    Matrica matrica(m, std::vector<int>(n,0));    
    if(m == 0 || n == 0) {
        std::cout << "Jedan od vektora je prazan!\n";
        return matrica;
    }

    for(int i = 0; i < m; i++) 
        for(int j = 0; j < n; j++) 
            matrica[i][j] = a[i]*b[j];

    return matrica;
}

int main() {
    int m,n;
    std::cout << "Unesite broj elemenata prvog vektora: ";
    std::cin >> m;
    std::cout << "Unesite elemente prvog vektora: ";
    std::vector<int> a(m, 0);
    for(int i = 0; i < m; i++) 
        std::cin >> a[i];
    
    std::cout << "Unesite broj elemenata drugog vektora: ";
    std::cin >> n;
    std::cout << "Unesite elemente drugog vektora: ";
    std::vector<int> b(n, 0);
    for(int i = 0; i < n; i++) 
        std::cin >> b[i];

    Matrica matrica = KroneckerovProizvod(a, b);
    int maxSirina = NajvecaSirina(matrica);

    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) {
            std::cout << std::setw(maxSirina) << matrica[i][j] << " ";
        }
        std::cout << "\n";
    }
    
}
