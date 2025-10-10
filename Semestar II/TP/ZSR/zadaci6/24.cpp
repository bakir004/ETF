#include <iostream>

int** NumerickaAlokacija(int n, int** &m1) {
    if(n <= 0) throw "Pogresan parametar";
    int dosadAlocirano = 0;
    try {
        m1 = new int*[n]{};
        for(int i = 0; i < n; i++, dosadAlocirano++)
            m1[i] = new int[n-i];
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n-i; j++)
                m1[i][j] = j+1;
    } catch(...) {
        for(int i = 0; i < dosadAlocirano; i++)
            delete[] m1[i];
        delete[] m1;
        throw "Kreiranje nije uspjelo";
    }
    int **m2 = nullptr;
    try {
        m2 = new int*[n]{};
        m2[0] = new int[n*(n+1)/2];
        for(int i = 1; i < n; i++)
            m2[i] = m2[i-1] + i;
        for(int i = 0; i < n; i++)
            for(int j = 0; j < i+1; j++)
                m2[i][j] = j+1;
    } catch(...) {
        delete[] m2;
        throw "Kreiranje nije uspjelo";
    }
    return m2;
}

int main() {
    int n;
    std::cin >> n;
    int **m1 = nullptr, **m2 = nullptr;
    try {
        m2 = NumerickaAlokacija(n, m1);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n-i; j++)
                std::cout << m1[i][j];
            std::cout << "\n";
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < i+1; j++)
                std::cout << m2[i][j];
            std::cout << "\n";
        }
        delete[] m2[0];
        delete[] m2;
        for(int i = 0; i < n; i++)
            delete[] m1[i];
        delete[] m1;
    } catch(...) {
        std::cout << "Problemi s memorijom";
    }
    
    return 0;
}