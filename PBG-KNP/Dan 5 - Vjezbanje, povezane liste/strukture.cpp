#include<iostream>

using namespace std;

struct Student {
  string ime, prezime;
  int razred;
};

int main() {
  Student s{"fatima", "plivcic", 3};
  cout << s.ime << " " << s.prezime << " je " << s.razred << ". razred";
  return 0;
}
