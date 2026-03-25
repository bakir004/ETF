#include<iostream>
#include<vector>

using namespace std;

int BrojKojiFali(vector<int> v) {
  int suma = 0;
  for(int i = 0; i < v.size(); i++)
    suma += v[i];
  int n = v.size()+1;
  int s = n*(n+1)/2;
  return s - suma;
}

int main() {
  vector<int> brojevi{2,1,4,5,8,9,7,6};
  cout << BrojKojiFali(brojevi);

  return 0;
}
