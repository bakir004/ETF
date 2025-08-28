#include<deque>
#include<iostream>
#include<algorithm>

template <typename T>
class RedSPrioritetom {
  std::deque<T> elementi;
  void Sortiraj() {
    std::sort(elementi.begin(), elementi.end());
  }
public:
  bool Prazan() const { return elementi.empty(); }
  void Dodaj(T x) {
    elementi.push_back(x);
    Sortiraj();
  }
  T DajNajveci() const { return elementi.back(); }
  void UkloniNajveci() { elementi.pop_back(); }
  int Velicina() { return elementi.size(); }
};

int main() {
  RedSPrioritetom<int> r;
  r.Dodaj(5);
  r.Dodaj(8);
  r.Dodaj(2);
  r.Dodaj(-3);
  
  while(!r.Prazan()) {
    std::cout << r.DajNajveci();
    r.UkloniNajveci();
  }

  return 0;
}
