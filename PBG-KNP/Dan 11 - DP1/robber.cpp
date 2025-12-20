#include<iostream>

using namespace std;

vector<int> memo;

int ukradi(vector<int>& kuce, int from) {
  if(from == kuce.size()-1)
    return kuce[from];
  if(from >= kuce.size())
    return 0;
  if(memo[from]) return memo[from];

  int izbor1 = kuce[from] + ukradi(kuce,from+2);
  int izbor2 = ukradi(kuce,from+1);

  return memo[from] = max(izbor1,izbor2);
}

int main() {
  vector<int> kuce;

  int n = 10000;
  for(int i = 1; i <= n; i++)
    kuce.push_back(i);

  memo = vector<int>(n+1, 0);

  cout << ukradi(kuce, 0);

  return 0;
}
