#include<iostream>
#include<string>
#include<map>

bool Permutacija(std::string s1, std::string s2) {
    std::map<char, int> chars;
    for(auto c : s1) chars[c]++;
    for(auto c : s2) chars[c]--;
    for(auto par : chars) if(par.second != 0) return false;
    return true;
}

int main() {
    std::string s1 = "bakir004";
    std::string s2 = "rikab040";
    std::cout << Permutacija(s1, s2);
    return 0;
}
