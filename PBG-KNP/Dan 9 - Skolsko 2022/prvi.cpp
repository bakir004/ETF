#include<iostream>

using namespace std;

void ispisiNajveciBrojBezNulaIUzastopnihCifara(int n) {
  int trenutnaCifra;
  if(n % 3 == 0 || n % 3 == 2) trenutnaCifra = 2;
  else trenutnaCifra = 1;

  while(n > 0) {
    n -= trenutnaCifra;
    cout << trenutnaCifra;
    trenutnaCifra = 3 - trenutnaCifra;
  }
}

int main() {
  int n;
  cin >> n;
  ispisiNajveciBrojBezNulaIUzastopnihCifara(n);

  return 0;
}
