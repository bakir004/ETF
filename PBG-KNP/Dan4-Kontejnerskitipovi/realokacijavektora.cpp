#include<iostream>
#include<vector>

int main() {
  std::vector<int> v;
  for(int i = 0; i < 100; i++) {
    v.push_back(i);
    std::cout << &v[0] << "\n";
  }
  return 0;
}
