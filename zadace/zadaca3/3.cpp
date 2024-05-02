#include <functional>
#include <iostream>
#include <map>
#include <iomanip>
#include <stdexcept>

enum class Smjer {Desno, Dolje, Dijagonalno};
typedef std::map<Smjer, std::function<int(int)>> MapaPreslikavanja;

template <typename TipElemenata>
struct Matrica {
    char ime_matrice;
    int br_redova, br_kolona;
    TipElemenata **elementi = nullptr;
};
template <typename TipElemenata>
void UnistiMatricu(Matrica<TipElemenata> &mat) {
    // std::cout << "Brisem matricu dimenzija " << mat.br_redova << "x" << mat.br_kolona << "\n";
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
void IspisiMatricu(Matrica<TipElemenata> mat, int sirina = 4, bool treba_brisati = false) {
    for(int i = 0; i < mat.br_redova; i++) {
        for(int j = 0; j < mat.br_kolona; j++)
            std::cout << std::setw(sirina) << mat.elementi[i][j];
        std::cout << std::endl;
    }
    if(treba_brisati) 
        UnistiMatricu(mat);
}
template <typename TipElemenata>
Matrica<TipElemenata> ZbirMatrica(const Matrica<TipElemenata> &m1, const Matrica<TipElemenata> &m2) {
    if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m1.br_kolona);
    for(int i = 0; i < m1.br_redova; i++)
        for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
    return m3;
}

Matrica<int> ProsiriPremaFunkcijama(Matrica<int> mat, MapaPreslikavanja m, int n) {
    if(n < 1) throw std::domain_error("Besmislen parametar");
    Matrica<int> novaMatrica = StvoriMatricu<int>(mat.br_redova, mat.br_kolona);
    for(int i = 0; i < mat.br_redova; i++) 
        for(int j = 0; j < mat.br_kolona; j++) 
            novaMatrica.elementi[i][j] = mat.elementi[i][j];
    mat = novaMatrica;

    if(n == 1) { return mat; }
    while(n > 1) {
        Matrica<int> prosirenaMatrica;
        try {
            if (!m[Smjer::Desno] && !m[Smjer::Dolje] && !m[Smjer::Dijagonalno]){
                return mat;
            } else if(m[Smjer::Desno] && !m[Smjer::Dolje] && !m[Smjer::Dijagonalno]) {
                prosirenaMatrica = StvoriMatricu<int>(mat.br_redova, mat.br_kolona*2);
                for(int i = 0; i < mat.br_redova; i++) 
                    for(int j = 0; j < mat.br_kolona; j++) 
                        prosirenaMatrica.elementi[i][j+mat.br_kolona] = m[Smjer::Desno](mat.elementi[i][j]);
            } else if(m[Smjer::Dolje] && !m[Smjer::Desno] && !m[Smjer::Dijagonalno]) {
                prosirenaMatrica = StvoriMatricu<int>(mat.br_redova*2, mat.br_kolona);
                for(int i = 0; i < mat.br_redova; i++) 
                    for(int j = 0; j < mat.br_kolona; j++) 
                        prosirenaMatrica.elementi[i+mat.br_redova][j] = m[Smjer::Dolje](mat.elementi[i][j]);
            } else { // prosirujemo na sve strane
                if(!m[Smjer::Desno]) m[Smjer::Desno] = [](int x){return x;};
                if(!m[Smjer::Dolje]) m[Smjer::Dolje] = [](int x){return x;};
                if(!m[Smjer::Dijagonalno]) m[Smjer::Dijagonalno] = [](int x){return x;};
                prosirenaMatrica = StvoriMatricu<int>(mat.br_redova*2, mat.br_kolona*2);
                for(int i = 0; i < mat.br_redova; i++) 
                    for(int j = 0; j < mat.br_kolona; j++) {
                        prosirenaMatrica.elementi[i][j+mat.br_kolona] = m[Smjer::Desno](mat.elementi[i][j]);
                        prosirenaMatrica.elementi[i+mat.br_redova][j] = m[Smjer::Dolje](mat.elementi[i][j]);
                        prosirenaMatrica.elementi[i+mat.br_redova][j+mat.br_kolona] = m[Smjer::Dijagonalno](mat.elementi[i][j]);
                    }
            }
        } catch(...) {
            UnistiMatricu(mat);
            throw;
        }

        // prepisivanje u originalni dio
        for(int i = 0; i < mat.br_redova; i++) 
            for(int j = 0; j < mat.br_kolona; j++) 
                prosirenaMatrica.elementi[i][j] = mat.elementi[i][j];

        UnistiMatricu(mat);
        mat = prosirenaMatrica;
        n--;
    }

    return mat;
}

int main() {
    int m, n;
    std::cout << "Unesite dimenzije matrice (redovi, kolone): ";
    std::cin >> m >> n;
    Matrica<int> matrica = StvoriMatricu<int>(m, n, 'A');
    UnesiMatricu(matrica);
    MapaPreslikavanja mapa;
    mapa[Smjer::Desno] = [](int x){ return x+1; };
    mapa[Smjer::Dolje] = [](int x){ return x+2; };
    mapa[Smjer::Dijagonalno] = [](int x){ return x+3; };
    Matrica<int> prosirena1 = ProsiriPremaFunkcijama(matrica, mapa, 3);
    IspisiMatricu(matrica, 10, 5, true);
    IspisiMatricu(prosirena1, 10, 5, true);
    return 0;
}
