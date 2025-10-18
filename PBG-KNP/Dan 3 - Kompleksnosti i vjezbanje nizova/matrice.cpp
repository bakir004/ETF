#include<iostream>
#include<vector>

using namespace std;

int main() {
  vector<vector<int>> matrica{{1,2,3},{4,5,6},{7,8,9}};

  int suma = 0;
  int vel = matrica.size();
  // for(int i = 0; i < matrica.size(); i++) {
  //   for(int j = 0; j < matrica[i].size(); j++) {
  //     if(i==j || i+j==vel-1)
  //       suma += matrica[i][j];
  //   }
  // }

  // za glavnu i sporednu
  for(int i = 0; i < vel; i++) {
    suma += matrica[i][i]; // glavna
    suma += matrica[i][vel-1-i]; // sporedna
  }
  if(vel % 2 == 1)
    suma -= matrica[vel/2][vel/2];

  cout << suma;

  return 0;
}
