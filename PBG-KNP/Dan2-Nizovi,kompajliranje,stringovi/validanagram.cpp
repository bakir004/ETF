#include<iostream>
#include<string>

using namespace std;

bool isAnagram(string s, string t) {
  int histogram1[26];
  int histogram2[26];
  for(int i = 0; i < 26; i++) {
    histogram1[i] = 0;
    histogram2[i] = 0;
  }
  for(int i = 0; i < s.size(); i++) {
    char slovo = s[i];
    histogram1[slovo-97]++;
  }
  for(int i = 0; i < t.size(); i++) {
    char slovo = t[i];
    histogram2[slovo-97]++;
  }
  for(int i = 0; i < 26; i++) {
    if(histogram1[i] != histogram2[i])
      return false;
  }
  return true;
}

int main() {

  return 0;
}
