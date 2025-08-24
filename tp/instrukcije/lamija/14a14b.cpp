#include<iostream>
#include<string>
#include<vector>

int main() {
  std::string s("bakir");
  const char* p = s.c_str();
  std::vector<int> v{1,2,3,4,5};
  int* pointer = v.data();

  return 0;
}
