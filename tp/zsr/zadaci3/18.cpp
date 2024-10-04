#include<iostream>
#include<string>
#include<vector>

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> Matrix;

bool IsMagicSquare(Matrix matrix) {
    int targetSum = 0, sum;
    // racunamo prvi red
    for(int n : matrix[0])
        targetSum += n;
    // racunamo ostale redove i poredimo
    for(int i = 1; i < matrix.size(); i++) {
        sum = 0;
        for(int j = 0; j < matrix[i].size(); j++) 
           sum += matrix[i][j]; 
        
        if(sum != targetSum) return false;
    }
    // racunamo sve kolone i poredimo
    for(int i = 0; i < matrix.size(); i++) {
        sum = 0;
        for(int j = 0; j < matrix[i].size(); j++) 
            sum += matrix[j][i]; 

        if(sum != targetSum) return false;
    }
    // racunamo glavnu dijagonalu
    sum = 0;
    for(int i = 0; i < matrix.size(); i++)
        sum += matrix[i][i];
    if(sum != targetSum) return false;

    // racunamo sporednu dijagonalu
    sum = 0;
    for(int i = 0; i < matrix.size(); i++)
        for(int j = 0; j < matrix.size(); j++)
            if(i + j == matrix.size()-1)
                sum += matrix[i][j];
    if(sum != targetSum) return false;

    return true;
}

int main() {
    Matrix m1{{2,7,6},{9,5,1},{4,3,8}};
    Matrix m2{{3,7,6},{9,5,1},{4,3,8}};
    std::cout << std::boolalpha << IsMagicSquare(m1) << " " << IsMagicSquare(m2);
    return 0;
}