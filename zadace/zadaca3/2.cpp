#include <algorithm>
#include <cctype>
#include <map>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

typedef std::tuple<std::string, int, int> Lokacija;
typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::map<std::string, std::vector<Lokacija>> IndeksPojmova;

void ToUpperStr(std::string &s) { std::transform(s.begin(), s.end(), s.begin(), [](char c){ return std::toupper(c); }); }

IndeksPojmova KreirajIndeksPojmova(Knjiga knjiga) {
    IndeksPojmova indeksPojmova;
    for(auto par : knjiga) {
        std::string trenutnoPoglavlje = par.first;
        std::vector<std::string> stranice = par.second;
        for(int j = 0; j < stranice.size(); j++) {
            std::string trenutnaStranica = stranice[j];
            int brojStranice = j+1;
            ToUpperStr(trenutnaStranica);
            std::string trenutnaRijec = "";
            int pozicijaRijeci = -1;
            for(int i = 0; i < trenutnaStranica.length(); i++) {
                if(std::isalnum(trenutnaStranica[i])) {
                    if(trenutnaRijec.length() == 0)
                        pozicijaRijeci = i+1;
                    trenutnaRijec += trenutnaStranica[i];
                } else if(trenutnaRijec.length() != 0) {
                    Lokacija lokacija{trenutnoPoglavlje, brojStranice, pozicijaRijeci};
                    indeksPojmova[trenutnaRijec].push_back(lokacija);
                    trenutnaRijec = "";
                }
            }
            if(trenutnaRijec.length() != 0) {
                Lokacija lokacija{trenutnoPoglavlje, brojStranice, pozicijaRijeci};
                indeksPojmova[trenutnaRijec].push_back(lokacija);
            }
        }
    }
    return indeksPojmova;
}

std::vector<Lokacija> PretraziIndeksPojmova(IndeksPojmova indeksPojmova, std::string rijec) {
    for(int i = 0; i < rijec.length(); i++)
        if(!std::isalnum(rijec[i]))
            throw std::domain_error("Neispravna rijec!");

    return indeksPojmova[rijec];
}

void IspisiIndeksPojmova(IndeksPojmova indeksPojmova) {
    std::cout << "\nKreirani indeks pojmova:\n";
    for(auto par : indeksPojmova) {
        std::string rijec = par.first;
        std::cout << rijec << ": ";
        std::vector<Lokacija> lokacije = par.second;
        for(int i = 0; i < lokacije.size(); i++) {
            if(i != 0) std::cout << ", ";
            std::cout << std::get<0>(lokacije[i]) << "/" << std::get<1>(lokacije[i]) << "/" << std::get<2>(lokacije[i]);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    Knjiga knjiga;
    std::string poglavlje;
    while(true) {
        std::cout << "Unesite naziv poglavlja: ";
        std::getline(std::cin, poglavlje);
        if(poglavlje.length() == 0) break;
        int brojStranice = 1;
        std::string stranica;
        while(true) {
            std::cout << "Unesite sadrzaj stranice " << brojStranice << ": ";
            std::getline(std::cin, stranica);
            if(stranica.length() == 0) break;
            knjiga[poglavlje].push_back(stranica);
            brojStranice++;
        }
    }
    IndeksPojmova indeksPojmova = KreirajIndeksPojmova(knjiga);
    IspisiIndeksPojmova(indeksPojmova);
    std::string rijec;
    while(true) {
        std::cout << "Unesite rijec: ";
        std::getline(std::cin, rijec);
        if(rijec.length() == 0) break;
        bool ispravno = true;
        for(int i = 0; i < rijec.length(); i++)
            if(!std::isalnum(rijec[i]))
                ispravno = false;
        
        if(!ispravno) {
            std::cout << "Neispravna rijec!\n";
            continue;
        }
        ToUpperStr(rijec);
        std::vector<Lokacija> lokacije = PretraziIndeksPojmova(indeksPojmova, rijec);
        if(lokacije.size() == 0) {
            std::cout << "Rijec nije nadjena!\n";
            continue;
        }
        std::cout << "Rijec nadjena na pozicijama: ";
        for(Lokacija lok : lokacije)
            std::cout << std::get<0>(lok) << "/" << std::get<1>(lok) << "/" << std::get<2>(lok) << " ";
        std::cout << "\n";
    }
    std::cout << "Dovidjenja!";
    return 0;
}
