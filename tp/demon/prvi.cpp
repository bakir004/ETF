#include <deque>
#include <iostream>

int main() {
  std::deque<int> d1{1, 2, 3, 4, 5};
  std::deque<int> d2{1, 2, 3, 4, 5};
  std::cout << (d1 == d2);
}
