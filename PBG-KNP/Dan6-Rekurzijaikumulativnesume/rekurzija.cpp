#include<iostream>

using namespace std;

int memo[1000000];

int fib(int n) {
  if(n <= 0) return -1;
  if(n == 1 || n == 2) return 1;
  if(memo[n] != 0)
    return memo[n];
  return memo[n] = fib(n-1) + fib(n-2);
}

// int fib(int n) {
//   if(n == 1 || n == 2) return 1;
//   return fib(n-1) + fib(n-2);
// }

int fibIterativno(int n) {
  if(n == 1 || n == 2) return 1;

  int trenutni = 0, prvi = 1, drugi = 1;

  for(int i = 3; i <= n; i++) {
    trenutni = prvi + drugi;
    drugi = prvi;
    prvi = trenutni;
  }
  return trenutni;
}

int fact(int n) {
  if(n < 0) return -1;
  if(n <= 1) return 1;
  return n * fact(n-1);
}

int fact2(int n) {
  if(n == 0) return 1;
  int m = 0;
  for(int i = 2; i <= n; i++)
    m*=i;
  return m;
}

int zbirCifara(int n) {
  if(n < 10) return n;
  int cifra = n % 10;
  return cifra + zbirCifara(n/10);
}

int main() {
  // memset(memo, 0, 1000000*sizeof(int));
  cout << zbirCifara(12345);
  // for(int i = 0; i < 100; i++) {
  //   cout << memo[i] << " ";
  // }
  return 0;
}
