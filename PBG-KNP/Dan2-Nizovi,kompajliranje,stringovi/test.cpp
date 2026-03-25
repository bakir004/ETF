#include<iostream>
#include<cmath>

using namespace std;

int ZbirCifara(int x) {
  int suma = 0;
  while(x > 0) {
    int cifra = x % 10;
    suma += cifra;
    x = x / 10;
  }
  return suma;
}

bool SavrsenBroj(long long x) {
  long long suma = 0;
  for(long long i = 1; i < x; i++) {
    if(x % i == 0) {
      suma += i;
    }
  }
  // if(suma == x) return true;
  // else return false;
  return suma == x;
}

int DigitalniKorijen(int x) {
  while(x >= 10) {
    x = ZbirCifara(x);
  }
  return x;
}

int BrojCifara(int x) {
  return log10(x) + 1;
}

int main() {
  cout << BrojCifara(1000000000);
  return 0;
}
