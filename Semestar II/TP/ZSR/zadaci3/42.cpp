#include <cctype>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string s;
    getline(std::cin, s);
    int words = 0, letters = 0;
    for(int i = 0; i < s.length(); i++) {
        if(isalpha(s[i])) letters++;
        if(s[i] == ' ' && i > 0 && s[i-1] != ' ') {
            std::cout << '\n';
            words++;
        } 
        else if(s[i] != ' ') std::cout << s[i];
    }
    if(s[s.length()-1] != ' ') words++;
    std::cout << "\nRijeci: " << words << ", slova: " << letters;
    return 0;
}