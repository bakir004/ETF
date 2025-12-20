#include<iostream>

using namespace std;

vector<long long> memo;

long long fib(long long n) {
  if(n <= 2) return 1;
  if(memo[n] != 0) return memo[n];
  return memo[n] = fib(n-1) + fib(n-2);
}

int main() {
  long long n = 90;
  memo = vector<long long>(n+2, 0);
  memo[1] = 1;
  memo[2] = 1;
  cout << "Fib " << n << " = " << fib(n); 
  return 0;
}
