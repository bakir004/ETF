#include<iostream>

using namespace std;

bool imaLiBroj(string s) {
  for(int i = 0; i < s.size(); i++)
    if(s[i] >= '0' && s[i] <= '9')
      return true;
  return false;
}
bool imaLiMaloSlovo(string s) {
  for(int i = 0; i < s.size(); i++)
    if(s[i] >= 'a' && s[i] <= 'z')
      return true;
  return false;
}
bool imaLiVelikoSlovo(string s) {
  for(int i = 0; i < s.size(); i++)
    if(s[i] >= 'A' && s[i] <= 'Z')
      return true;
  return false;
}
bool imaLiSpecijalni(string s) {
  string spec = "!@#$%^&*()-+";
  for(int i = 0; i < s.size(); i++) {
    char trenutni = s[i];
    for(int j = 0; j < spec.size(); j++)
      if(trenutni == spec[j])
        return true;
  }
  return false;
}

int minimumNumber(string sifra, int n) {
  int brojKarakteraKojeTrebaDodat = 0;
  if(!imaLiSpecijalni(sifra)) brojKarakteraKojeTrebaDodat++;
  if(!imaLiMaloSlovo(sifra)) brojKarakteraKojeTrebaDodat++;
  if(!imaLiVelikoSlovo(sifra)) brojKarakteraKojeTrebaDodat++;
  if(!imaLiBroj(sifra)) brojKarakteraKojeTrebaDodat++;

  if(n + brojKarakteraKojeTrebaDodat >= 6) 
    return brojKarakteraKojeTrebaDodat;
  else return 6 - n;
  // .#bA2
}

int main() {
  cout << minimumNumber("2bbbb", 5) << endl;
  cout << minimumNumber("2bb#A", 5) << endl;
  cout << minimumNumber("Er1", 3) << endl;
  cout << minimumNumber("#HackerRank", 11) << endl;
  return 0;
}
