#include <iostream>
#include <fstream>
#include <functional>
#include <stdexcept>

template <typename TipElemenata>
void SortirajBinarnuDatoteku(const char ime_datoteke[],
        std::function<bool(TipElemenata, TipElemenata)> kriterij
        = std::less<TipElemenata>()) {
    std::fstream tok(ime_datoteke, std::ios::in | std::ios::out | std::ios::binary);
    if(!tok) throw std::logic_error("Datoteka ne postoji");
    tok.seekg(0, std::ios::end);
    int velicina = tok.tellg() / sizeof(TipElemenata);
    for(int i = 0; i < velicina; i++) {
        for(int j = i+1; j < velicina; j++) {
            std::streampos pozicijaI = i * sizeof(TipElemenata);
            tok.seekg(pozicijaI, std::ios::beg);
            TipElemenata elementI;
            tok.read(reinterpret_cast<char*>(&elementI), sizeof(TipElemenata));
            std::streampos pozicijaJ = j * sizeof(TipElemenata);
            tok.seekg(pozicijaJ, std::ios::beg);
            TipElemenata elementJ;
            tok.read(reinterpret_cast<char*>(&elementJ), sizeof(TipElemenata));
            if(!kriterij(elementI, elementJ)) {
                tok.seekp(pozicijaI, std::ios::beg);
                tok.write(reinterpret_cast<char*>(&elementJ), sizeof(TipElemenata));
                tok.seekp(pozicijaJ, std::ios::beg);
                tok.write(reinterpret_cast<char*>(&elementI), sizeof(TipElemenata));
            }
            if(tok.bad()) throw std::logic_error("Problemi u pristupu datoteci");
        }
    }
}

int main() {
    std::ofstream izlaz("brojevi.dat", std::ios::binary);
    for(int i = 5; i >= 1; i--) 
        izlaz.write(reinterpret_cast<char*>(&i), sizeof i);
    izlaz.close();
    SortirajBinarnuDatoteku<int>("brojevi.dat");
    std::ifstream ulaz("brojevi.dat", std::ios::binary);
    for(int i = 0; i < 5; i++) {
        int broj;
        ulaz.read(reinterpret_cast<char*>(&broj), sizeof(broj));
        std::cout << broj << " ";
    }

    return 0;
}
