#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

std::string IzdvojiRijec(std::string recenica, int n) {
    int counter = 0;
    bool razmak = true;
    if(n < 1)
        throw std::range_error("Pogresan redni broj rijeci!");
    std::string rijec = "";
    for(int i = 0; i < recenica.length(); i++) {
        if(!isgraph(recenica[i]))
            razmak = true;
        else {
            if(razmak) {
                counter++;
                razmak = false;
            }
            if(counter == n) 
                rijec += recenica[i];
        }
    }
    if(rijec.length() == 0) 
        throw std::range_error("Pogresan redni broj rijeci!");
   
    return rijec;
}

int main() {
    int n;
    std::cout << "Unesite redni broj rijeci: ";
    std::cin >> n;
    std::cout << "Unesite recenicu: ";
    std::string recenica;
    std::cin.clear();
    std::cin.ignore(32767, '\n');
    std::getline(std::cin, recenica);
    try {
        std::string rijec = IzdvojiRijec(recenica, n);
        std::cout << "Rijec na poziciji " << n << " je " << rijec << "\n";
    } catch(const std::range_error& err) {
        std::cout << "IZUZETAK: " << err.what();
    }
}
