#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#define SLOVA_U_ABECEDI 26

char ShiftajSlovo(char slovo, char slovoKljuca) {
    int kolicinaPomjeranja = slovoKljuca - 'A';   
    int pocetnoSlovo = islower(slovo) ? 'a' : 'A';
    return (slovo - pocetnoSlovo + kolicinaPomjeranja) % SLOVA_U_ABECEDI + pocetnoSlovo;
}

char ShiftajSlovoUnazad(char slovo, char slovoKljuca) {
    int kolicinaPomjeranja = slovoKljuca - 'A';   
    int pocetnoSlovo = islower(slovo) ? 'a' : 'A';
    int D = slovo - pocetnoSlovo - kolicinaPomjeranja;
    if(D < 0) D += SLOVA_U_ABECEDI;
    return D += pocetnoSlovo;
}

std::string ViginereSifriranje(std::string poruka, std::string kljuc) {
    for(auto c : kljuc)
        if(c < 'A' || c > 'Z') throw std::domain_error("Neispravan kljuc");
    int counter = 0;
    for(int i = 0; i < poruka.length(); i++) {
        if(isalpha(poruka.at(i))) {
            // modulo operator se koristi za wrap-around ideju iteriranja kroz karaktere kljuca
            poruka.at(i) = ShiftajSlovo(poruka.at(i), kljuc.at(counter % kljuc.length()));
            counter++;
        }
    }
    
    return poruka;
}

std::string ViginereDesifriranje(std::string poruka, std::string kljuc) {
    for(auto c : kljuc)
        if(c < 'A' || c > 'Z') throw std::domain_error("Neispravan kljuc");
    int counter = 0;
    for(int i = 0; i < poruka.length(); i++) {
        if(isalpha(poruka.at(i))) {
            // modulo operator se koristi za wrap-around ideju iteriranja kroz karaktere kljuca
            poruka.at(i) = ShiftajSlovoUnazad(poruka.at(i), kljuc.at(counter % kljuc.length()));
            counter++;
        }
    }
    
    return poruka;
}

bool ValidanKljuc(std::string kljuc) {
    if(kljuc.length() == 0) return false;
    for(auto c : kljuc) 
        if(c < 'A' || c > 'Z') 
            return false;

    return true;
}

int main() {
    std::string kljuc;
    while(true) {
        bool bad = false;
        std::cout << "Unesite kljuc: ";
        std::getline(std::cin, kljuc);
        if(!ValidanKljuc(kljuc))
            std::cout << "Neispravan kljuc, ponovite unos:\n";
        else 
            break;
    }
    std::string poruka;
    std::cout << "Kljuc prihvacen. Unesite poruku: ";
    std::getline(std::cin, poruka);

    std::string sifrirano = ViginereSifriranje(poruka, kljuc);
    std::cout << "Sifrirana poruka: " << sifrirano << "\n\n";

    while(true) {
        std::string noviKljuc;
        std::cout << "Pokusajte pogoditi kljuc: ";
        std::getline(std::cin, noviKljuc);
        if(!ValidanKljuc(noviKljuc))
            std::cout << "Niste pogodili kljuc, probajte ponovo!\n";
        else {
            std::string desifrirano = ViginereDesifriranje(sifrirano, noviKljuc);
            std::cout << "Pokusaj desifriranja: " << desifrirano << "\n";
            if(desifrirano == poruka) {
                std::cout << "Bravo, pogodili ste kljuc!";
                break;
            } else std::cout << "Niste pogodili kljuc, probajte ponovo:\n";
        }
    }
    return 0;
}
