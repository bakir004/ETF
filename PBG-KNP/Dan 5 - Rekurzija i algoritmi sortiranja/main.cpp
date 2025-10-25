#include<iostream>

using namespace std;

int brojMimoilazenja(vector<char>& auta) {
  int brojDesnih = 0, mimoilazenja = 0;
  for(auto a : auta) {
    if(a == 'D')
      brojDesnih++;
    else 
      mimoilazenja+=brojDesnih;
  }
  return mimoilazenja;
}

int main() {
  vector<char> v{'L','D','D','L','D','L','L'};
  cout << brojMimoilazenja(v);
  return 0;
}
