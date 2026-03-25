#include<iostream>
#include<string>
#include<vector>

int FindSubstring(std::string haystack, std::string needle) {
    int matchingLetters = 0;
    for(int i = 0; i < haystack.length(); i++) {
        if(haystack[i] == needle[matchingLetters]) {
            matchingLetters++;
            if(matchingLetters == needle.length()) {
                if((haystack[i - needle.length()] == ' ' || i - needle.length() == -1) && (haystack[i+1] == ' ' || i+1 == haystack.size()))
                return i - needle.length() + 1;
            }
        }
        else {
            matchingLetters = 0;
        }
    }
    return -1;
}


std::string ZamjenaRijeci(std::string s, std::vector<std::string> toReplace, std::vector<std::string> replacements) {
    for(int i = 0; i < toReplace.size(); i++) {
        int indexOfWord;
        std::string word = toReplace[i];
        while(indexOfWord = FindSubstring(s, word), indexOfWord != -1) {
            std::string reconstructedS = s.substr(0, indexOfWord);
            reconstructedS += replacements[i];
            reconstructedS += s.substr(indexOfWord + word.length(), 999999999);
            s = reconstructedS;
        }
    }
    return s;
}

int main() {
    std::string s1 = "sta je tu je";
    std::cout << ZamjenaRijeci(s1, {"sta", "je", "tu"}, {"what", "is", "there"});
    std::cout << "\n";
    std::string s2 = "ko rano rani dvije srece grabi";
    std::cout << ZamjenaRijeci(s2, {"ko", "rano", "rani", "dvije", "srece", "grabi"}, {"who", "early", "earlies", "two", "lucks", "grabs"});
    return 0;
}