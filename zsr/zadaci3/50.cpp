#include<iostream>
#include<string>

std::string IzbrisiPodstring(std::string s1, std::string s2) {
    int matchingLetters = 0;
    for(int i = 0; i < s1.length(); i++) {
        if(s1[i] == s2[matchingLetters]) {
            matchingLetters++;
            if(matchingLetters == s2.length()) {
                s1.erase(i - s2.length()+1, s2.length());
                i -= s2.length();
            }
        }
        else {
            matchingLetters = 0;
        }
    }
    return s1;
}

int main() {
    std::string s1 = "na vrh brda vrba brda", s2 = "brda";
    std::string res = IzbrisiPodstring(s1, s2);
    std::cout << res;
    return 0;
}