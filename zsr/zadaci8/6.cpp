#include <algorithm>
#include <iostream>
#include <string>
#include <map>

std::string Sifriraj(std::string poruka, std::map<char,char> kljuc) {
    std::string sifrirano = "";
    std::for_each(poruka.begin(), poruka.end(), [&sifrirano, &kljuc](char c){sifrirano += (kljuc[c] ? kljuc[c] : c);});
    return sifrirano;
}

int main() {
    std::map<char,char> kljuc;
    kljuc['B'] = 'L';
    kljuc['A'] = 'E';
    kljuc['K'] = 'V';
    kljuc['I'] = 'A';
    kljuc['R'] = 'T';
    std::cout << Sifriraj("BAKIR", kljuc);
    return 0;
}
