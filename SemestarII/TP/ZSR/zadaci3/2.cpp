#include<iostream>
#include<vector>

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> Matrix;

int IndexOfBiggestColumn(Matrix matrix) {
    int maxSum = -1, index = 1;
    for(int i = 0; i < matrix.size(); i++) {
        int sum = 0;
        for(int j = 0; j < matrix[i].size(); j++)
            sum += matrix[j][i];
        if(sum > maxSum) {
            maxSum = sum;
            index = i+1;
        }
    }
    return index;
}

int IndexOfSmallestRow(Matrix matrix) {
    int maxSum = -1, index = 1;
    for(int i = 0; i < matrix.size(); i++) {
        int sum = 0;
        for(int j = 0; j < matrix[i].size(); j++)
            sum += matrix[i][j];
        if(sum > maxSum) {
            maxSum = sum;
            index = i+1;
        }
    }
    return index;
}

int DiagonalSum(Matrix matrix) {
    int sum = 0;
    for(int i = 0; i < matrix.size(); i++)
        sum += matrix[i][i];
    return sum;
}

int main() {
    int n;
    std::cin >> n;
    Matrix matrix(n, VectorInt(n));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            std::cin >> matrix[i][j];
    int smallestRowNumber = IndexOfSmallestRow(matrix);
    int biggestColumnNumber = IndexOfBiggestColumn(matrix);
    int diagonalSum = DiagonalSum(matrix);
    std::cout << "Najmanji red je " << smallestRowNumber << "., najveca kolona je " << biggestColumnNumber << "., a zbir na dijagonali je " << diagonalSum;
    return 0;
}