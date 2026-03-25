#include<iostream>

typedef std::vector<std::vector<double>> Matrica;

Matrica UnesiMatricu() {
    Matrica matrica;
    while(true) {
        std::vector<double> red;
        while(true) {
            int unos;
            std::cin >> unos;
            if(!std::cin) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                break;
            }
            red.push_back(unos);
        }
        if(red.size() == 0) break;
        matrica.push_back(red);
    }
    return matrica;

}

std::vector<double> DajProsjekeRedovaMatrice(Matrica m) {
    std::vector<double> prosjeci;
    for(int i = 0; i < m.size(); i++) {
        double suma = 0;
        for(int j = 0; j < m[i].size(); j++) {
            suma += m[i][j];
        }
        double prosjek = suma / m[i].size();
        prosjeci.push_back(prosjek);
    }
    return prosjeci;
}

int main() {
    Matrica matrica = UnesiMatricu();
    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) {
            std::cout << matrica[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::vector<double> prosjeci = DajProsjekeRedovaMatrice(matrica);
    for(int i = 0; i < prosjeci.size(); i++) {
        std::cout << prosjeci[i] << " ";
    }
    return 0;
}

// 1 2 3 .
// 2 .
// 5 6 7 8 9 .
// .
//
// 1 2 3
// 2
// 5 6 7 8 9
