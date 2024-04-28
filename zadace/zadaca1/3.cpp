#include <iostream>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>

std::vector<long long int> BrojeviUStringu(std::string s) {
    long long int temp = 0;
    bool negative = false;
    std::vector<long long int> res;
    for(int i = 0; i < s.length(); i++) {
        // ako je cifra
        if(std::isdigit(s.at(i))) {
            bool validNum = true;
            // temp je trenutni broj koji gradimo
            // ako je -1 onda trebamo tek poceti da ga gradimo
            if(temp == -1) {
                if(s.at(i-1) == '-')
                    negative = true; // dodacemo '-' kasnije
                temp = 0;
                // prije nego sto ga izgradimo, da prodjemo do kraja broja da vidimo
                // ima li slucajno slova na kraju
                int counter = i;
                while(counter < s.length() && std::isdigit(s.at(counter)))
                    counter++;
                if(counter != s.length() && std::isalpha(s.at(counter))) {
                    validNum = false;
                    i = --counter; // ako ima slovo na kraju velikog broja, preskocicemo ga
                }
            }
            if(!validNum)
                continue;

            int c = s.at(i) - '0';
            if(temp > (LONG_LONG_MAX - c)/10) throw std::range_error("Prevelik broj");
            temp = temp * 10 + c;
        } else if(std::isalpha(s.at(i))) {
            // ako naidjemo na slovo, broj koji smo prethodno gradili invalidiramo
            negative = false;
            temp = -1;
        } else if(temp != -1){
            // ako naidjemo na nesto mimo slova ili broja, broj koji smo dosad
            // gradili dodaj u vektor pronadjenih brojeva
            if(negative)
                temp = -temp;
            res.push_back(temp);
            negative = false;
            temp = -1;
        }
    }
    if(temp != -1) {
        if(negative)
            temp = -temp;
        res.push_back(temp);
    }
    return res;
}

int main() {
    // 9223372036854775807
    // Reperi 2PAC i 50Cent znaju da zbir brojeva 327, 39 i -4162 iznosi -3796! Oni također znaju da 123xy nije broj, mada 456 jeste. Šta se može reći za 0?
    std::string s;
    std::cout << "Unesite string: ";
    std::getline(std::cin, s);
    try {
        std::vector<long long int> res = BrojeviUStringu(s);
        if(res.size() == 0)
            std::cout << "Uneseni string ne sadrzi nijedan broj!";
        else {
            std::cout << "Brojevi unutar stringa: ";
            for(int i = 0;i < res.size(); i++)
                std::cout << res.at(i) << " ";
        }
    } catch(std::range_error err) {
        std::cout << "PROBLEM: Uneseni string sadrzi prevelik broj!!!";
    }
    return 0;
}
