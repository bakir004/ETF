#include<iostream>
#include<string>

using namespace std;

string decToBin(int n, int brojCifara) {
  string rez = "";
  while(n > 0) {
    int ostatak = n % 2;
    n /= 2;
    rez = (char)('0'+ostatak) + rez;
    brojCifara--;
  }
  for(int i = 0; i < brojCifara; i++)
    rez = '0' + rez;
  return rez;
}

int main() {
  vector<pair<int,int>> grahovi{{9,4},{3,8},{9,2},{1,11},{2,11}};

  int n = grahovi.size();

  int najmanjaRazlika = INT_MAX;
  string najboljiIzbor;
  for(int i = 1; i < pow(2,n); i++) {
    string binarno = decToBin(i,n);
    int slatkoca = 0, gorcina = 1;
    for(int j = 0; j < n; j++) {
      if(binarno[j] == '1') {
        slatkoca += grahovi[j].second;
        gorcina *= grahovi[j].first;
      }
    }
    if(abs(slatkoca - gorcina) < najmanjaRazlika) {
      najmanjaRazlika = abs(slatkoca - gorcina);
      najboljiIzbor = binarno;
    }
  }
  cout << "Razlika: " << najmanjaRazlika << "\n";
  cout << najboljiIzbor;


  return 0;
}
