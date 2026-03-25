#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

bool Prost(int n) {
  if(n < 0) return false;
  int korijen = sqrt(n);
  for(int i = 2; i <= korijen; i++)
    if(n % i == 0) 
      return false;
  return true;
}

int diagonalDifference(vector<vector<int>>& matrica) {
  int sumaGlavna = 0, sumaSporedna = 0;
  int n = matrica.size();

  for(int i = 0; i < n; i++)
    if(Prost(matrica[i][i]))
      sumaGlavna+=matrica[i][i];

  for(int i = 0; i < n; i++)
    if(Prost(matrica[i][n-1-i]))
      sumaSporedna+=matrica[i][n-1-i];

  // for(int i = 0; i < matrica.size(); i++) {
  //   for(int j = 0; j < matrica[i].size(); j++) {
  //     if(i == j && Prost(matrica[i][j]))
  //       sumaGlavna += matrica[i][j];
  //     if(i + j == matrica.size()-1 && Prost(matrica[i][j]))
  //       sumaSporedna += matrica[i][j];
  //   }
  // }
  return sumaGlavna-sumaSporedna;
}

int main() {
  int n;
  cin >> n;
  vector<vector<int>> matrica(n, vector<int>(n, 0));

  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      cin >> matrica[i][j];

  cout << diagonalDifference(matrica);

  return 0;
}
