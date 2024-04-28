#include<iostream>
#include<string>

int main() {
    std::string s1, s2;
    std::getline(std::cin, s1);
    std::getline(std::cin, s2);
    std::cout << "1. recenica sadrzi " << s1.length() << " znakova, a 2. sadrzi " << s2.length() << " znakova.\n";
    int redoslijed = s2.length() < s1.length() + 1;
    for(int i = 0; i < std::min(s1.length(), s2.length()); i++) {
        if(s1[i] < s2[i]) redoslijed = 1;
        else if(s2[i] < s1[i]) redoslijed = 2;
    }
    std::cout << redoslijed << ". abecedno dolazi prva\n";
    std::cout << "Sastavljene: " << s1 + s2 << "\n";
    std::cout << "Obrnuta prva: ";
    for(int i = s1.length() - 1; i >= 0; i--)
        std::cout << s1[i];
    return 0;
}