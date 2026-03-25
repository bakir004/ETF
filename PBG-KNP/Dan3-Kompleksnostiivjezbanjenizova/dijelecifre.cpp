#include<iostream>
#include<vector>

using namespace std;

bool dijeleCifre(int a, int b) { // a = 1223296, b = 95712
  vector<bool> vidjen(10, false);
  while(a > 0) {
    int cifra = a % 10;
    vidjen[cifra] = true;
    a /= 10;
  }
  while(b > 0) {
    int cifra = b % 10;
    if(vidjen[cifra])
      return true;
    b /= 10;
  }
  return false;
}

int main() {

  return 0;
}
