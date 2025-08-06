#include <iostream>
#include <stdexcept>
#include <utility>
typedef std::vector<std::vector<int>> Matrica;

void ObrniElemente(std::vector<int>& v) {
    for(int i = 0; i < v.size()/2; i++) {
        std::swap(v[i], v[v.size()-1-i]);
    }
}

std::deque<int> TransformisiMatricuIDajSumuKolona(Matrica& m) {
    for(int i = 0; i < m.size()-1; i++) {
        if(m[i].size() != m[i+1].size()) {
            throw std::domain_error("Grbava matrica");
        }
    }

    std::deque<int> dek;
    for(int i = 0; i < m.size(); i++) {
        ObrniElemente(m[i]);
    }
    for(int i = 0; i < m.size(); i++) {
        int suma = 0;
        for(int j = 0; j < m[i].size(); j++) {
            suma += m[j][i];

            // m[0][0]
            // m[1][0]
            // m[2][0]
            // m[3][0]
            // m[4][0]
        }
        dek.push_back(suma);
    }
    return dek;
}

int main() {
    // std::vector<int> v{1,4,8,3,6};
    // ObrniElemente(v);
    // for(int i = 0; i < v.size(); i++) {
    //     std::cout << v[i] << " ";
    // }

    Matrica m{{1,2,3},{4,5,6},{7,8,9}};
    try {
        std::deque<int> sumePoKolonama = TransformisiMatricuIDajSumuKolona(m);
        std::cout << "Sume po kolonama:\n";
        for(int i = 0; i < sumePoKolonama.size(); i++) {
            std::cout << sumePoKolonama[i] << " ";
        }
    } catch(std::domain_error e) {
        std::cout << e.what() << "\n";
    }
    std::cout << "\nMatrica:\n";
    for(int i = 0; i < m.size(); i++) {
        for(int j = 0; j < m[i].size(); j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << "\n";
    }
    

    return 0;
}
