#include<iostream>
#include<algorithm>
#include<vector>
#include<string>

using namespace std;

string pretvoriUBin(int x) {
  string bin = "";
  while(x > 0) {
    int ostatak = x % 2;
    x /= 2;
    bin = char('0'+ostatak) + bin;
  }
  return bin;
}

int pretraziNiz(vector<int> niz, int trazeni) { // O(n)
  for(int i = 0; i < niz.size(); i++)
    if(niz[i] == trazeni) return i;
  return -1;
}

bool imaDuplikata(vector<int> niz) { // O(n^2)
  for(int i = 0; i < niz.size(); i++) {
    int trenutniKojiTrazim = niz[i];
    for(int j = i+1; j < niz.size(); j++) {
      if(trenutniKojiTrazim == niz[j])
        return true;
    }
  }
  return false;
}

bool imaDuplikata2(vector<int> niz) { // kompleksnost je O(n log n)
  sort(niz.begin(),niz.end());

  for(int i = 0; i < niz.size(); i++) {
    if(niz.at(i) == niz.at(i+1))
      return true;
  }
  return false;
}

int main() {
  // vector<int> niz{1,7,3,46,99,83,4};
  cout << pretvoriUBin(74);
  return 0;
}
