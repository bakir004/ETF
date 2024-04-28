#include <cctype>
#include <iostream>
#include <string>
#include <vector>

void LowercaseString(std::string &str) {
    for(int i = 0; i < str.length(); i++) 
        str[i] = tolower(str[i]);
}   

void IzdvojiKrajnjeRijeci(std::vector<std::string> &rijeci, std::string &najmanji, std::string &najveci) {
    najmanji = rijeci[0];
    najveci = rijeci[0];
    for(int i = 1; i < rijeci.size(); i++) {
        std::string trenutni = rijeci[i];
        LowercaseString(trenutni);
        if(trenutni.compare(najmanji) < 0)
            najmanji = trenutni;
        if(trenutni.compare(najveci) > 0)
            najveci = trenutni;
    }
}

void ZadrziDuplikate(std::vector<std::string> &rijeci) {
    bool found;
    for(int i = 0; i < rijeci.size(); i++) {
        found = false;
        for(int j = 0; j < rijeci.size(); j++) {
            if(i != j && rijeci[i].compare(rijeci[j]) == 0) {
                found = true;
                break;
            }
        }
        if(!found) {
            std::string temp = rijeci[i];
            rijeci[i] = rijeci[rijeci.size() - 1];
            rijeci[rijeci.size() - 1] = temp; 
            rijeci.pop_back();
            i--;
        }
    }
    for(int i = 0; i < rijeci.size(); i++) {
        found = false;
        for(int j = i+1; j < rijeci.size(); j++) {
            if(rijeci[i].compare(rijeci[j]) == 0) {
                std::string temp = rijeci[i];
                rijeci[i] = rijeci[rijeci.size() - 1];
                rijeci[rijeci.size() - 1] = temp; 
                rijeci.pop_back();
                i--;
            }
        }
    }
}

int main() {
    int n;
    std::cout << "Koliko zelite unijeti rijeci: ";
    std::cin >> n;
    std::vector<std::string> rijeci;
    std::cout << "Unesite rijeci: ";
    for(int i = 0; i < n; i++) {
        std::string rijec;
        std::cin >> rijec;
        rijeci.push_back(rijec);
    }
    std::string minimum, maximum;

    IzdvojiKrajnjeRijeci(rijeci, minimum, maximum); 
    ZadrziDuplikate(rijeci);
    
    std::cout << "\nPrva rijec po abecednom redu je: " << minimum << "\nPosljednja rijec po abecednom redu je: " << maximum << "\n";
    std::cout << "Rijeci koje se ponavljaju su: ";

    for(int i = 0; i < rijeci.size(); i++) 
        std::cout << rijeci[i] << " ";

    return 0;
}




















