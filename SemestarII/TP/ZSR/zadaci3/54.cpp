#include<iostream>
#include<string>

int Substring(std::string haystack, std::string needle) {
    int matchingLetters = 0;
    for(int i = 0; i < haystack.length(); i++) {
        if(haystack[i] == needle[matchingLetters]) {
            matchingLetters++;
            if(matchingLetters == needle.length()) {
                return i - needle.length() + 1;
            }
        }
        else {
            matchingLetters = 0;
        }
    }
    return -1;
}

int main() {
    std::string s1, s2;
    std::getline(std::cin, s1);
    std::getline(std::cin, s2);
    std::cout << Substring(s1,s2);
    return 0;
}