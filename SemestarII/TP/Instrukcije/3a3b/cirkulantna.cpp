#include<iostream>

typedef std::vector<std::vector<double>> Matrica;

bool DaLiJeCirkulantna(Matrica m) {
    for(int i = 1; i < m.size(); i++) {
        int velicinaReda = m[i].size();
        if(m[i][0] != m[i-1][velicinaReda-1]) {
            return false;
        }
        for(int j = 1; j < velicinaReda; j++) {
            if(m[i][j] != m[i-1][j-1])
                return false;
        }
    }

    return true;
}

int main() {
    Matrica matrica{{1,2,3},{3,1,2},{2,3,1}};
    for(int i = 0; i < matrica.size(); i++) {
        for(int j = 0; j < matrica[i].size(); j++) {
            std::cout << matrica[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Cirkulantna: " << DaLiJeCirkulantna(matrica);
    
    return 0;
}
