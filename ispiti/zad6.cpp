#include <cctype>
#include <vector>
#include <string>
#include <iostream>

std::vector<int> NadjiBrojeve(std::string s) {
    std::vector<int> brojevi;
    int trenutniBroj = -1;
    for(int i = 0; i < s.length(); i++) {
        if((i == 0 && std::isdigit(s[i]) || (i > 0 && std::isdigit(s[i]) && s[i-1] == ' ')))
            trenutniBroj = s[i]-'0';               
        else if(std::isdigit(s[i])) {
            if(trenutniBroj == -1) {
                while(std::isdigit(s[i]))
                    i++;
                i--;
            } else {
                trenutniBroj = trenutniBroj * 10 + s[i]-'0';
            }
        } else if(s[i] == ' ' || std::ispunct(s[i])) {
            if(trenutniBroj != -1)
                brojevi.push_back(trenutniBroj);
            trenutniBroj = -1;
        } else {
            trenutniBroj = -1;
        }
    }
    return brojevi;
}

int main() {
    std::string s = "120 hrabrih studenata izašlo na ispit, 35 ih je odustalo, ostalo ih 85...";
    std::string s2 = "2PAC i U2 imaju koncert";
    std::vector<int> v = NadjiBrojeve(s);
    for(auto e : v)
        std::cout << e << " ";
    return 0;
}
