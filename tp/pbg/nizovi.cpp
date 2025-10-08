#include<iostream>
#include<vector>
#include<string>

using namespace std;

// bool Palindrom(string s) { // neefikasno
//   string obrnut = s;
//   reverse(obrnut.begin(), obrnut.end());
//   return s == obrnut;
// }

bool Palindrom(string s) {
  int velicina = s.size();
  for(int i = 0; i < velicina; i++) {
    if(s[i] != s[velicina-1-i]) {
      return false;
    }
  }
  return true;
}

string Obrni(string s) {
  string obrnuti = "";
  for(int i = s.size()-1; i >= 0; i--) {
    obrnuti += s[i];
    // obrnuti.push_back(s[i]);
  }
  return obrnuti;
}

int main() {
  // string str = "bakir";
  //
  // int vel = str.size();
  // for(int i = 0; i < vel/2; i++) {
  //   char temp = str[i];
  //   str[i] = str[vel-i-1];
  //   str[vel-i-1] = temp;
  // }
  // cout << str;

  // int a = 3, b = 5;
  // int temp = a;
  // a = b;
  // b = temp;

  // int niz[10];
  // for(int i = 0; i < 10; i++) {
  //   niz[i] = 0;
  // }
  //
  // for(int i = 0; i < 10; i++) {
  //   cout << niz[i] << " ";
  // }


  // vector<int> v{1,2,3,4,5};
  // v.push_back(6);
  // v.erase(v.begin() + 2);
  //
  // for(int i = 0; i < v.size(); i++) {
  //   cout << v[i] << " ";
  // }

  // string s = "hello world";
  //
  // for(int i = 0; i < s.size(); i++) {
  //   cout << s[i] << " ";
  // }

  // racecar, level, 

  return 0;
}
