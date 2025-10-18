#include<iostream>
#include<map>

using namespace std;

int main() {
  int n;
  cin >> n;
  map<int, string> pravila{
    {3,"fizz"},
    {5,"buzz"},
    {7,"bezz"},
    {11, "bum"}
  };

  for(int i = 1; i <= n; i++) {
    string s;
    for(pair<int,string> par : pravila) { // moze i (auto par:pravila)
      if(i % par.first == 0)
        s += par.second;
    }
    if(s == "") cout << i << endl;
    else cout << s << endl;
  }

  return 0;
}
