#include<iostream>
#include<cstring>

class String {
public:
  char* znakovi;
  int duzina;
  String(): znakovi(nullptr), duzina(0) {}
  String(char* niz): znakovi(new char[strlen(niz)+1]), duzina(strlen(niz)) {
    for(int i = 0; i <= duzina; i++)
      znakovi[i] = niz[i];
  }
  String(const String& s): znakovi(new char[s.duzina+1]), duzina(s.duzina) {
    for(int i = 0; i <= duzina; i++)
      znakovi[i] = s[i];
  }
  String(String&& s): znakovi(s.znakovi), duzina(s.duzina) { }
  String& operator=(const String& s) {
    delete[] znakovi;
    znakovi = new char[s.duzina+1];
    for(int i = 0; i < s.duzina; i++)
      znakovi[i] = s[i];
    duzina = s.duzina;
    return *this;
  }
  String& operator=(String&& s) {
    znakovi = s.znakovi;
    duzina = s.duzina;
    return *this;
  }
  int DajDuzinu() const { return duzina; }
  char operator[](int index) const { return znakovi[index]; }
  char& operator[](int index) { return znakovi[index]; }
  String& operator+=(const String& s) {
    char* novi = new char[duzina + s.duzina + 1];
    for(int i = 0; i < duzina; i++)
      novi[i] = znakovi[i];
    for(int i = duzina; i < duzina + s.duzina+1; i++)
      novi[i] = s.znakovi[i-duzina];
    delete[] znakovi;
    znakovi = novi;
    duzina = duzina + s.duzina;
    return *this;
  }
  friend std::ostream& operator<<(std::ostream& tok, const String& s);
  ~String() { delete[] znakovi; }
};

String operator+(const String& s1, const String& s2) {
  String s;
  s += s1;
  s += s2;
  return s;
}

std::ostream& operator<<(std::ostream& tok, const String& s) {
  std::cout << s.znakovi;
  // for(int i = 0; i < s.duzina; i++)
  //   tok << s.znakovi[i];
  return tok;
}

int main() {
  char c[7] = "Hello ";
  char c2[6] = "world";
  String s(c);
  String s2(c2);
  String s3 = s + s2;
  std::cout << s3;

  return 0;
}
