#include<iostream>

using namespace std;

bool DaLiJeCirkulantna(deque<deque<int>> matrica) {
  deque<int> red = matrica.front();

  for(int i = 1; i < matrica.size(); i++) {
    int zadnji = red.back();
    red.pop_back();
    red.push_front(zadnji);
    if(red != matrica[i])
      return false;
  }
  return true;
}

int main() {
  deque<deque<int>> matrica{{1,2,3},{3,1,2},{1,2,3}};
  cout << DaLiJeCirkulantna(matrica);
  return 0;
}
