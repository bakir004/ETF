#include <iostream>

long long int** StirlingoviBrojeviV1(int vel) {
    long long int **matrica = nullptr;
    int dosadAlocirano = 0;
    try {
        matrica = new long long int*[vel+1]{};
        for(int i = 0; i < vel+1; i++, dosadAlocirano++)
            matrica[i] = new long long int[i+1];

        matrica[0][0] = 1;
        for(int n = 1; n < vel+1; n++)
            matrica[n][0] = 0;
        for(int n = 1; n < vel+1; n++) 
            for(int k = 1; k <= n; k++) 
                matrica[n][k] = matrica[n-1][k-1] + k * (n-1<k ? 0:matrica[n-1][k]);
    } catch(...) {
        for(int i = 0; i < dosadAlocirano; i++)
            delete[] matrica[i];
        delete[] matrica;
        throw;
    }
    return matrica;
}

long long int** StirlingoviBrojeviV2(int vel) {
    long long int **matrica = nullptr;
    try {
        matrica = new long long int*[vel+1]{};
        matrica[0] = new long long int[(vel+1)*(vel+2)/2];
        for(int i = 1; i < vel+1; i++)
            matrica[i] = matrica[i-1] + i;

        matrica[0][0] = 1; // greska u njegovom ispisu, njemu pise 0 za 0,0 a treba 1
        for(int n = 1; n < vel+1; n++)
            matrica[n][0] = 0;
        for(int n = 1; n < vel+1; n++)
            for(int k = 1; k <= n; k++) 
                matrica[n][k] = matrica[n-1][k-1] + k * (n-1<k ? 0:matrica[n-1][k]);
    } catch(...) {
        delete[] matrica[0];
        delete[] matrica;
        throw;
    }
    return matrica;
}


int main() {
    int n;
    std::cin >> n;
    long long int **matrica1 = nullptr, **matrica2 = nullptr;
    try {
        matrica1 = StirlingoviBrojeviV1(n);
        for(int i = 0; i < n+1; i++) {
            for(int j = 0; j < i+1; j++)
                std::cout << matrica1[i][j] << " ";
            std::cout << "\n";
        }
        matrica2 = StirlingoviBrojeviV2(n);
        for(int i = 0; i < n+1; i++) {
            for(int j = 0; j < i+1; j++)
                std::cout << matrica2[i][j] << " ";
            std::cout << "\n";
        }
        for(int i = 0; i < n+1; i++) 
            delete[] matrica1[i];
        delete[] matrica1;
        delete[] matrica2[0];
        delete[] matrica2;
    } catch(...) {
        delete[] matrica1;
        std::cout << "Error";
    }
    return 0;
}