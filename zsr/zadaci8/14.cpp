#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <new>
template <typename TipElemenata>
struct Matrica {
    char ime_matrice; // Koristi se samo u funkciji "UnesiMatricu"
    int br_redova, br_kolona;
    TipElemenata **elementi = nullptr; // VEOMA BITNA INICIJALIZACIJA!!!
};
template <typename TipElemenata>
void UnistiMatricu(Matrica<TipElemenata> &mat) {
    if(!mat.elementi) return;
    for(int i = 0; i < mat.br_redova; i++) delete[] mat.elementi[i];
    delete[] mat.elementi;
    mat.elementi = nullptr;
}
template <typename TipElemenata>
Matrica<TipElemenata> StvoriMatricu(int br_redova, int br_kolona, char ime = 0) {
    Matrica<TipElemenata> mat;
    mat.br_redova = br_redova; mat.br_kolona = br_kolona; mat.ime_matrice = ime;
    mat.elementi = new TipElemenata*[br_redova]{};
    try {
        for(int i = 0; i < br_redova; i++)
            mat.elementi[i] = new TipElemenata[br_kolona];
    }
    catch(...) {
        UnistiMatricu(mat);
        throw;
    }
    return mat;
}
template <typename TipElemenata>
void UnesiMatricu(Matrica<TipElemenata> &mat) {
    for(int i = 0; i < mat.br_redova; i++)
        for(int j = 0; j < mat.br_kolona; j++) {
            std::cout << mat.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
            std::cin >> mat.elementi[i][j];
        }
}
template <typename TipElemenata>
void IspisiMatricu(Matrica<TipElemenata> mat, int sirina_ispisa, int preciznost, bool treba_brisati) {
    for(int i = 0; i < mat.br_redova; i++) {
        for(int j = 0; j < mat.br_kolona; j++)
            std::cout << std::setw(sirina_ispisa) << std::setprecision(preciznost) << mat.elementi[i][j];
        std::cout << std::endl;
    }
    if(treba_brisati)
        UnistiMatricu(mat);
}
template <typename TipElemenata>
Matrica<TipElemenata> ZbirMatrica(const Matrica<TipElemenata> &m1,
        const Matrica<TipElemenata> &m2) {
    if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m1.br_kolona);
    for(int i = 0; i < m1.br_redova; i++)
        for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
    return m3;
} 
template <typename T>
Matrica<T> ProduktMatrica(const Matrica<T> &m1, const Matrica<T> &m2) {
    Matrica<T> produkt = StvoriMatricu<T>(m1.br_redova, m2.br_kolona);
    for(int i = 0; i < m1.br_redova; i++) 
        for(int j = 0; j < m2.br_kolona; j++) 
            for(int k = 0; k < m1.br_kolona; k++) 
                produkt.elementi[i][j] += m1.elementi[i][k] * m2.elementi[k][j];
    return produkt;
}

template<typename T>
Matrica<T> MnoziSkalarom(const Matrica<T> &mat, int skalar) {
    Matrica<T> rezultat = StvoriMatricu<T>(mat.br_kolona, mat.br_redova);
    for(int i = 0; i < mat.br_redova; i++)
        for(int j = 0; j < mat.br_kolona; j++)
            rezultat.elementi[i][j] = mat.elementi[i][j]*skalar;
    return rezultat;
}

template <typename T>
Matrica<T> MatricniPolinom(const Matrica<T> &A, std::vector<int> koeficijenti) {
    if(A.br_kolona != A.br_redova) throw std::domain_error("Matrica mora biti kvadratna");
    Matrica<T> rezultat = StvoriMatricu<T>(A.br_redova, A.br_kolona);
    if(koeficijenti.size() == 0) {
        for(int i = 0; i < A.br_redova; i++)
            for(int j = 0; j < A.br_kolona; j++)
                rezultat.elementi[i][j] = 0;
        return rezultat;
    }
    Matrica<T> stepenA = StvoriMatricu<T>(A.br_redova, A.br_kolona);
    for(int i = 0; i < A.br_redova; i++)
        for(int j = 0; j < A.br_kolona; j++)
            stepenA.elementi[i][j] = (i == j ? 1 : 0);

    for(int i = 0; i < koeficijenti.size(); i++) {
        Matrica<T> clan = MnoziSkalarom(stepenA, koeficijenti[i]);
        Matrica<T> zbirRezultataIClana = ZbirMatrica(rezultat, clan);
        rezultat = zbirRezultataIClana;
        Matrica<T> noviStepenA = ProduktMatrica(stepenA, A);
        stepenA = noviStepenA;

        UnistiMatricu(clan);
        UnistiMatricu(zbirRezultataIClana);
        UnistiMatricu(noviStepenA);
    }
    UnistiMatricu(stepenA);
    return rezultat;
}

int main() {
    Matrica<double> a; // AUTOMATSKA INICIJALIZACIJA!!!
    int n;
    std::cout << "Unesi dimenziju kvadratne matrice:\n";
    std::cin >> n;
    try {
        std::cout << "Unesite elemente matrice A:\n";
        a = StvoriMatricu<double>(n, n);
        UnesiMatricu(a);
        std::cout << "Unesite red polinoma: ";
        int red;
        std::cin >> red;
        std::vector<int> koeficijenti(red+1);
        std::cout << "Unesite koeficijente polinoma: ";
        for(int i = 0; i < red+1; i++)
            std::cin >> koeficijenti[i];
        IspisiMatricu(MatricniPolinom(a, koeficijenti), 10, 6, true);
    }
    catch(std::bad_alloc) {
        std::cout << "Nema dovoljno memorije!\n";
    }
    UnistiMatricu(a);
    return 0;
}
