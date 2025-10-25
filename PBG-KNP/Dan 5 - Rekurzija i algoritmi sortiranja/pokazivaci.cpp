#include<iostream>

using namespace std;

struct Cvor {
  int vrijednost;
  Cvor* sljedeci;
};

int main() {
  Cvor* pocetak = new Cvor{1,nullptr};
  Cvor* prosli = pocetak;
  for(int i = 2; i <= 10; i++) {
    Cvor* novi = new Cvor{i,nullptr};
    prosli->sljedeci = novi;
    prosli = novi;
  }

  // prolaz kroz povezanu listu
  Cvor* pomocni = pocetak;
  while(pomocni != nullptr) {
    cout << pomocni->vrijednost << " ";
    pomocni = pomocni->sljedeci;
  }





  // int a = 5;
  // int* p = &a;
  // cout << *p;

  // int* p = new int(6);
  // radim sa tim pointerom
  // delete p;
  return 0;
}
