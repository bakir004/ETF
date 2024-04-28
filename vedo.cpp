#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>

std::string Konverzija(int n, int baza, int padding) {
    std::map<int, std::string> cifre;
    cifre[2] = "01";
    cifre[8] = "01234567";
    cifre[16] = "0123456789ABCDEF";
    if(cifre[baza] == "")
        throw std::range_error("Nepodrzana baza za konverziju");

    std::string rez;
    while(n > 0) {
        rez = cifre[baza].at(n % baza) + rez;
        n /= baza;
    }
    while(rez.length() < padding)
        rez = '0' + rez;

    return rez;
}

std::vector<std::vector<std::string>> Konverzije(std::vector<int> niz) {
    if(niz.size() == 0) throw std::domain_error("Ulazni niz ne smije biti prazan");
    std::vector<std::vector<std::string>> konvertovano(niz.size(), std::vector<std::string>());
    for(int i = 0; i < niz.size(); i++) {
        if(niz.at(i) < 0) throw std::range_error("Ulazni niz ne smije sadrzavati negativne elemente");

        konvertovano.at(i).push_back(Konverzija(niz.at(i), 2, 8));
        konvertovano.at(i).push_back(Konverzija(niz.at(i), 8, 0));
        konvertovano.at(i).push_back(Konverzija(niz.at(i), 16, 0));
    }
    return konvertovano;
}

int main() {
    int n;
    std::vector<int> niz;
    std::cout << "Unesite elemente niza (-1 za izlaz): ";
    while(std::cin >> n) {
        if(n == -1) break;
        niz.push_back(n);
    }
    try {
        std::vector<std::vector<std::string>> konvertovano = Konverzije(niz);
        std::cout << "     BIN    OKT  HEX\n";
        for(auto v : konvertovano){
            std::cout << std::right << std::setw(8) << v.at(0) << " "; 
            std::cout << std::right << std::setw(6) << v.at(1) << " "; 
            std::cout << std::right << std::setw(4) << v.at(2) << "\n"; 
        }
    } catch(std::domain_error err) {
        std::cout << err.what() << "\n";
    } catch(std::range_error err) {
        std::cout << err.what() << "\n";
    }
    return 0;
}

