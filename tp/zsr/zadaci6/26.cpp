#include <cstring>
#include <vector>
#include <iostream>
#include <string>

int PotencijalniKrivci(char** &krivci, std::vector<std::string> imena) {
    krivci = nullptr;
    int dosadAlocirano = 0;
    try {
        krivci = new char*[imena.size()]{};
        for(int i = 0; i < imena.size(); i++, dosadAlocirano++) {
            krivci[i] = new char[imena[i].length() + 1];
            for(int j = 0; j < imena[i].length(); j++)
                krivci[i][j] = imena[i][j];
            krivci[i][imena[i].size()] = '\0';
        }
    } catch(...) {
        for(int i = 0; i < dosadAlocirano; i++)
            delete[] krivci[i];
        delete[] krivci;
        throw;
    }
    return imena.size();
}

int OdbaciOptuzbu(char **&krivci, int vel, std::string ime) {
    int brojNul = 0;
    for(int i = 0; i < vel; i++) {
        if(krivci[i] != nullptr && std::equal(krivci[i], krivci[i] + strlen(krivci[i]), ime.begin())) {
            delete[] krivci[i];
            krivci[i] = nullptr;
        }
        if(krivci[i] == nullptr)
            brojNul++;
    }
    int novaVel = vel;
    if(brojNul <= 10)
        return novaVel;
    char **noviKrivci = nullptr;
    try {
        noviKrivci = new char*[novaVel]{};
        int index = 0;
        for(int i = 0; i < vel; i++) {
            if(krivci[i] != nullptr) {
                noviKrivci[index] = krivci[i];
                index++;
            }
        }
        delete[] krivci;
        krivci = noviKrivci;
        novaVel = vel-brojNul;
    } catch(...) {}
    return novaVel;
}

int DodajOptuzbu(char **&krivci, int vel, std::string ime) {
    char *imePok = nullptr;
    try {
        imePok = new char[ime.size()+1];
        for(int i = 0; i < ime.size(); i++)
            imePok[i] = ime[i];
        imePok[ime.size()] = '\0';
    } catch(...) {
        throw;
    }
    for(int i = 0; i < vel; i++) {
        if(krivci[i] == nullptr) {
            krivci[i] = imePok;
            return vel;
        }
    }
    char** noviKrivci = nullptr;
    try {
        noviKrivci = new char*[vel+1]{};
        for(int i = 0; i < vel; i++)
            noviKrivci[i] = krivci[i];
        noviKrivci[vel] = imePok;
        delete[] krivci;
        krivci = noviKrivci;
        return vel+1;
    } catch(...) {
        throw;
    }
    return vel;
}

void IzlistajOptuzbu(char **krivci, int vel) {
    for(int i = 0; i < vel; i++) {
        if(krivci[i] != nullptr)
            std::cout << krivci[i] << "\n";
        else 
            std::cout << "_\n"; 
    }
}

int main() {
    char** krivci = nullptr;
    std::vector<std::string> imena{
        "Zeljko", "Nermin", "Smajke", "MirzaHamza", "Nermina", 
        "Zeljko", "Nermin", "Smajke", "MirzaHamza", "Nermina", 
        "Zeljko", "Nermin", "Smajke", "MirzaHamza", "Nermina"
    };
    try {
        int vel = PotencijalniKrivci(krivci, imena);
        vel = OdbaciOptuzbu(krivci, vel, std::string("Zeljko"));
        vel = DodajOptuzbu(krivci, vel, std::string("Bakeroni"));
        vel = OdbaciOptuzbu(krivci, vel, std::string("Nermin"));
        vel = OdbaciOptuzbu(krivci, vel, std::string("Smajke"));
        IzlistajOptuzbu(krivci, vel);
        for(int i = 0; i < vel; i++) 
            delete[] krivci[i];
        delete[] krivci;
    } catch(...) {
        std::cout << "GRESKA";
    }
    return 0;
}