#include <iostream>
#include <stdexcept>

void KreirajMatricu(double **&matrica, int vel) {
    int dosadAlocirano = 0;
    try {
        matrica = new double*[vel]{};
        for(int i = 0; i < vel; i++, dosadAlocirano++)
            matrica[i] = new double[vel];
    } catch(...) {
        for(int i = 0; i < dosadAlocirano; i++)
            delete[] matrica[i];
        delete matrica;
        matrica = nullptr;
        throw std::logic_error("Nedovoljno memorije");
    }
}

void UnesiMatricu(double **&matrica, int vel) {
    std::cout << "Unesite elemente matrice (" << vel  << "x" << vel << "):\n";
    for(int i = 0; i < vel; i++)
        for(int j = 0; j < vel; j++)
            std::cin >> matrica[i][j];
}

void UnistiMatricu(double **&matrica, int vel) {
    for(int i = 0; i < vel; i++) 
        delete[] matrica[i];
    delete[] matrica;
    matrica = nullptr;
}

double **SaberiMatrice(double **&m1, double **&m2, int vel) {
    double **rez = nullptr;
    KreirajMatricu(rez, vel);
    for(int i = 0; i < vel; i++)
        for(int j = 0; j < vel; j++)
            rez[i][j] = m1[i][j] + m2[i][j];
    return rez;
}

void IspisiMatricu(double **&matrica, int vel) {
    std::cout << "Matrica:\n";
    for(int i = 0; i < vel; i++) {
        for(int j = 0; j < vel; j++)
            std::cout << matrica[i][j] << " ";
        std::cout << "\n";
    }
}

int main() {
    int n;
    std::cout << "Unesite dimenzije kvadratne matrice: ";
    std::cin >> n;
    double **m1 = nullptr;
    KreirajMatricu(m1, n);
    UnesiMatricu(m1, n);
    IspisiMatricu(m1, n);
    std::cout << "INT ";

    double **m2 = nullptr;
    KreirajMatricu(m2, n);
    UnesiMatricu(m2, n);
    IspisiMatricu(m2, n);

    double **m3 = SaberiMatrice(m1, m2, n);
    IspisiMatricu(m3, n);

    UnistiMatricu(m3, n);
    UnistiMatricu(m2, n);
    UnistiMatricu(m1, n);

    return 0;
}