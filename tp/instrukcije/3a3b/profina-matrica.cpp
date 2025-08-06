#include <iostream>
#include <iomanip>
#include <vector>

typedef std::vector<std::vector<double>> Matrica;

// Kreira matricu sa zadanim brojem redova i kolona
Matrica KreirajMatricu(int br_redova, int br_kolona) {
    Matrica matrica(br_redova, std::vector<double>(br_kolona));
    return matrica;
}

// Vraća broj redova zadane matrice
int BrojRedova(Matrica m) {
    return m.size();
}

// Vraća broj kolona zadane matrice
int BrojKolona(Matrica m) {
    return m[0].size();   
}

// Unosi sa tastature matricu sa zadanim brojem redova i kolona i
// vraća je kao rezultat
Matrica UnesiMatricu(int br_redova, int br_kolona) {
    Matrica matrica = KreirajMatricu(br_redova, br_kolona);

    for(int i = 0; i < BrojRedova(matrica); i++) {
        for(int j = 0; j < BrojKolona(matrica); j++) {
            std::cout << "Unesi element na poziciji (" << i << "," << j << "): ";
            std::cin >> matrica[i][j]; 
        }
    }

    return matrica;
}

// Ispisuje zadanu matricu
void IspisiMatricu(Matrica m, int sirina_ispisa) {
    for(int i = 0; i < BrojRedova(m); i++) {
        for(int j = 0; j < BrojKolona(m); j++) {
            std::cout << std::setw(sirina_ispisa) << m[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// Testira da li su dvije matrice saglasne za sabiranje
bool MoguSeSabirati(Matrica m1, Matrica m2) {
    // if(BrojRedova(m1) == BrojRedova(m2) && BrojKolona(m1) == BrojKolona(m2)) {
    //     return true;
    // }
    // return false;
    return BrojRedova(m1) == BrojRedova(m2) && BrojKolona(m1) == BrojKolona(m2);
}

// Vraca zbir zadanih matrica, uz pretpostavku da se mogu sabirati
Matrica SaberiMatrice(Matrica m1, Matrica m2) {
    Matrica m3 = KreirajMatricu(BrojRedova(m1), BrojKolona(m1));
    for(int i = 0; i < BrojRedova(m3); i++) {
        for(int j = 0; j < BrojKolona(m3); j++) {
            m3[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return m3;
}

// Testira da li su dvije matrice saglasne za množenje
bool MoguSeMnoziti(Matrica m1, Matrica m2) {
    return BrojKolona(m1) == BrojRedova(m2);
}

Matrica PomnoziMatrice(Matrica m1, Matrica m2) {
    Matrica m3 = KreirajMatricu(BrojRedova(m1), BrojKolona(m2));

    for(int i = 0; i < BrojRedova(m3); i++) {
        for(int j = 0; j < BrojKolona(m3); j++) {
            double suma = 0;
            for(int k = 0; k < BrojKolona(m1); k++) {
                suma += m1[i][k] * m2[k][j];
            }
            m3[i][j] = suma;
        }
    }

    return m3;
}

int main() {
    std::cout << "Unesite prvu matricu\n";
    Matrica matrica1 = UnesiMatricu(2, 2);
    std::cout << "Unesite drugu matricu\n";
    Matrica matrica2 = UnesiMatricu(2, 2);

    std::cout << "Matrica 1:\n";
    IspisiMatricu(matrica1, 6);
    std::cout << "Matrica 2:\n";
    IspisiMatricu(matrica2, 6);

    if(MoguSeMnoziti(matrica1, matrica2)) {
        Matrica proizvod = PomnoziMatrice(matrica1, matrica2);
        std::cout << "Proizvod:\n";
        IspisiMatricu(proizvod, 6);
    } else {
        std::cout << "Matrice se ne mogu pomnoziti!";
    }
    return 0;
}
