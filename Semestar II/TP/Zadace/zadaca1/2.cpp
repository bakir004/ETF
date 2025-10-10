#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

typedef std::vector<std::vector<double>> Matrica;

bool UGranicama(std::vector<int> velicine, int i, int j) {
    if(i < 0 || i >= velicine.size()) return false;
    if(j < 0 || j >= velicine.at(i)) return false;
    return true;
}

double Medijan(std::vector<double> niz) {
    int n = niz.size();
    // selection sort
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(niz.at(i) > niz.at(j)) {
                double temp = niz.at(i);
                niz.at(i) = niz.at(j);
                niz.at(j) = temp;
            }
        }
    }

    if(n % 2 == 0) return (niz.at(n/2) + niz.at(n/2-1)) / 2;
    else return niz.at(n/2);
}

Matrica MedijanskiFilter(Matrica matrica, int N) {
    if(N < 0) throw std::domain_error("Neispravan red filtriranja");

    Matrica filtriranaMatrica;
    std::vector<int> velicineMatrice; // ovaj vektor ce nam pomoci u provjeri jesmo li u granicama
    for(int i = 0; i < matrica.size(); i++) {
        filtriranaMatrica.push_back(std::vector<double>(matrica.at(i).size()));
        velicineMatrice.push_back(matrica.at(i).size());
    }
    // velicineMatrice vektor ima formu:
    // indeks koji opisuje indeks reda matrice: vrijednost koja opisuje koliko taj red ima elemenata
    // ovakva konstrukcija olaksava rad funkcije UGranicama()

    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica.at(i).size(); j++) {
            std::vector<double> pikseli;

            for(int k = i - N; k <= i + N; k++) 
                for(int l = j - N; l <= j + N; l++) 
                    if(UGranicama(velicineMatrice, k, l))
                        pikseli.push_back(matrica.at(k).at(l));

            filtriranaMatrica.at(i).at(j) = Medijan(pikseli);
        }
    }
    return filtriranaMatrica;
}

void IspisiMatricu(Matrica matrica) {
    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica.at(i).size(); j++)
            std::cout << std::setw(7) << std::fixed << std::setprecision(2) << matrica.at(i).at(j);
        std::cout << "\n";
    }
}

int main() {
    Matrica matrica;
    int m, n;
    double unos;
    std::cout << "Unesite broj redova i kolona matrice: ";
    std::cin >> m >> n;
    std::cout << "Unesite elemente matrice:\n";
    for(int i = 0; i < m; i++) {
        std::vector<double> red;
        for(int j = 0; j < n; j++) {
            std::cin >> unos;
            red.push_back(unos);
        }
        matrica.push_back(red);
    }
    int N;
    std::cout << "Unesite red filtriranja: ";
    std::cin >> N;
    try {
        Matrica filtrirano = MedijanskiFilter(matrica, N);
        std::cout << "\nMatrica nakon filtriranja:\n";
        IspisiMatricu(filtrirano);
    } catch(std::domain_error err) {
        std::cout << "\nGRESKA: " << err.what();
    }
    return 0;
}
