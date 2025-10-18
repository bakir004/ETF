#include<iostream>
#include<unordered_map>
#include<set>

using namespace std;

int main() {
  unordered_map<int,int> uspjeh;
  set<int> brojevi;
  uspjeh[5] = 14;
  uspjeh[1] = 6;
  uspjeh[4] = 2;
  uspjeh[3] = 8;
  uspjeh[2] = 2;

  for(auto& e : uspjeh) {
    cout << e.first << " ";
  }

  // brojevi.insert(2);
  // brojevi.insert(3);
  // brojevi.insert(4);
  // brojevi.insert(1);
  // brojevi.insert(2);
  //
  // cout << brojevi.size();
  // for(auto e : brojevi) {
  //   cout << e << " ";
  // }

  return 0;
}
