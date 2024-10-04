#include <cctype>
#include <cstring>
#include <iostream>

bool DaLiJePalindrom(const char *niz) {
    char *pocetak = (char*)niz, *kraj = pocetak + strlen(niz) - 1;
    while(pocetak < kraj) {
        if(std::toupper(*pocetak) != std::toupper(*kraj))
            return false;
        
        pocetak++;
        kraj--;
        while(!isalpha(*pocetak++));
        while(!isalpha(*kraj--));
    }
    return true;
}

int main() {
    std::cout << "Unesite string: ";
    char s[1000];
    std::cin.getline(s, sizeof s);
    std::cout << "Palindrom: " << std::boolalpha << DaLiJePalindrom(s);
    return 0;
}