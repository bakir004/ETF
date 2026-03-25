#include<iostream>
#include<vector>

using namespace std;

double Medijan(vector<double> v) {
  if(v.size() % 2 == 1)
    return v[v.size()/2];

  return (v[v.size()/2-1]+v[v.size()/2]) / 2;
}

int main() {
  vector<double> v{1,2,10,17,18};
  cout << Medijan(v);

  return 0;
}
