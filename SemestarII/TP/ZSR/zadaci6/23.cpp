#include <iostream>

char** CharAlokacija(int n, char** &m2) {
    if(n <= 0) throw "Pogresan parametar";
    char** m1 = nullptr;
    try {
        m1 = new char*[n]{};
        m1[0] = new char[n*(n+1)/2];
        for(int i = 1; i < n; i++)
            m1[i] = m1[i-1] + i;
        for(int i = 0; i < n; i++)
            for(int j = 0; j < i+1; j++)
                m1[i][j] = '*';
    } catch(...) {
        delete[] m1;
        throw "Kreiranje nije uspjelo";
    }
    int dosadAlocirano = 0;
    try {
        m2 = new char*[n]{};
        for(int i = 0; i < n; i++, dosadAlocirano++)
            m2[i] = new char[n-i];
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n-i; j++)
                m2[i][j] = '*';
    } catch(...) {
        for(int i = 0; i < dosadAlocirano; i++)
            delete[] m2[i];
        delete[] m2;
        throw "Kreiranje nije uspjelo";
    }
    return m1;
}

int main() {
    int n;
    std::cin >> n;
    char **m1 = nullptr, **m2 = nullptr;
    try {
        m1 = CharAlokacija(n, m2);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < i+1; j++)
                std::cout << m1[i][j];
            std::cout << "\n";
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n-i; j++)
                std::cout << m2[i][j];
            std::cout << "\n";
        }
        delete[] m1[0];
        delete[] m1;
        for(int i = 0; i < n; i++)
            delete[] m2[i];
        delete[] m2;
    } catch(...) {
        std::cout << "Problemi s memorijom";
    }
    
    return 0;
}