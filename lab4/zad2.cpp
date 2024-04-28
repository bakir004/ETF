#include <iostream>
#include <string>

void IzvrniString(std::string &str) {
    for(int i = 0; i < str.length()/2; i++) {
        char temp = str[i];
        str[i] = str[str.length()-1-i];
        str[str.length()-1-i] = temp;
    }
}

int main() {
    std::string str = "bakir Cinjarevic je najjaci programer...";
    IzvrniString(str);
    std::cout << str;
}
