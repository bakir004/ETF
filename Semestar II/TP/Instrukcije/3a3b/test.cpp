#include<iostream>
#include<vector>

typedef std::vector<std::vector<int>> Matrica;

Matrica KreirajMatricu(int brojRedova, int brojKolona) {
    Matrica matrica(brojRedova, std::vector<int>(brojKolona));
    return matrica;
}

int main() {
    Matrica matrica = KreirajMatricu(5, 4);

    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) {
            std::cout << matrica[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
