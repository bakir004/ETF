#include<iostream>

using namespace std;

pair<int,int> najboljaRazlika(int trenutni, vector<int>& v) {
  int minRazlika = INT_MAX;
  int index = 0;
  for(int i = 0; i < v.size(); i++) {
    if(abs(trenutni-v[i]) < minRazlika) {
      minRazlika = abs(trenutni-v[i]);
      index = i;
    }
  }
  return {minRazlika,index};
}

bool gotov(vector<bool>& gotovi) {
  for(auto e : gotovi)
    if(!e) return false;
  return true;
}

int main() {
  int n;
  cin >> n;
  vector<int> v1(n,0);
  vector<int> v2(n,0);

  for(int i = 0; i < n; i++)
    cin >> v1[i];
  for(int i = 0; i < n; i++)
    cin >> v2[i];

  //                  p1 z1 p2 z2
  vector<bool> gotovi{0, 0, 0, 0};
  int ukupnaRazlika = 0;

  int trenutni = v1[0];
  pair<int,int> razlika = najboljaRazlika(trenutni, v2);
  int minRazlika = razlika.first;
  ukupnaRazlika += minRazlika;
  int index = razlika.second;
  gotovi[0] = true;
  if(index == 0) gotovi[2] = true;
  else if(index == n-1) gotovi[3] = true;

  trenutni = v1[n-1];
  razlika = najboljaRazlika(trenutni, v2);
  minRazlika = razlika.first;
  ukupnaRazlika += minRazlika;
  index = razlika.second;
  gotovi[1] = true;
  if(index == 0) gotovi[2] = true;
  else if(index == n-1) gotovi[3] = true;
  if(gotov(gotovi)) {
    cout << ukupnaRazlika;
    return 0;
  }
  if(!gotovi[2]) {
    trenutni = v2[0];
    razlika = najboljaRazlika(trenutni, v1);
    minRazlika = razlika.first;
    ukupnaRazlika += minRazlika;
    index = razlika.second;
    gotovi[2] = true;
  }
  if(gotov(gotovi)) {
    cout << ukupnaRazlika;
    return 0;
  }
  if(!gotovi[3]) {
    trenutni = v2[n-1];
    razlika = najboljaRazlika(trenutni, v1);
    minRazlika = razlika.first;
    ukupnaRazlika += minRazlika;
    index = razlika.second;
    gotovi[3] = true;
  }
  if(gotov(gotovi)) {
    cout << ukupnaRazlika;
    return 0;
  }
  

  return 0;
}
