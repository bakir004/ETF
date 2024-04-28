#include <climits>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <complex>

typedef std::vector<double> VectorDouble;
typedef std::vector<VectorDouble> Matrix;

std::complex<double> MatrixToComplex(Matrix matrix) {
    int currSize = matrix[0].size();
    for(int i = 1; i < matrix.size(); i++)
        if(matrix[i].size() != currSize)
            throw std::domain_error("Parametar nema formu matrice");

    int minColIndex = 0, maxColIndex = 0;
    double minColProduct = INT_MAX, maxColProduct = -1;
    for(int i = 0; i < matrix[0].size(); i++) {
        double currProduct = 1;
        for(int j = 0; j < matrix.size(); j++)
            currProduct *= matrix[j][i];
        
        if(currProduct < minColProduct) {
            minColProduct = currProduct;
            minColIndex = i;
        }
        if(currProduct > maxColProduct) {
            maxColProduct = currProduct;
            maxColIndex = i;
        }
    }

    for(int i = 0; i < matrix.size(); i++) {
        auto temp = matrix[i][minColIndex];
        matrix[i][minColIndex] = matrix[i][maxColIndex];
        matrix[i][maxColIndex] = temp;
    }

    for(auto v : matrix) {
        for(auto n : v)
            std::cout << n << " ";
        std::cout << "\n";
    }

    return std::complex<double>(minColProduct, maxColProduct);
}

int main() {
    int n, m;
    std::cin >> m >> n;

    Matrix matrix(m, VectorDouble(n));
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            std::cin >> matrix[i][j];

    std::complex<double> z = MatrixToComplex(matrix);
    std::cout << z;

    return 0;
}
