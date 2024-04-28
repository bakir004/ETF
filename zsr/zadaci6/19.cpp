#include <iostream>
#include <stdexcept>
#include <string>

void DodajStudenta(std::string **&studenti, int &vel, std::string ime) {
    try {
        for(int i = 0; i < vel; i++) {
            if(studenti[i] == nullptr) {
                studenti[i] = new std::string(ime);
                return;
            }
        }
    } catch(...) {
        throw std::logic_error("Nedovoljno memorije");
    }
    std::string **prosireniStudenti = nullptr;
    const int prosirivac = 10;
    try {
        prosireniStudenti = new std::string*[vel+prosirivac]{};
        for(int i = 0; i < vel+prosirivac; i++) {
            if(i < vel) prosireniStudenti[i] = studenti[i];
            else prosireniStudenti[i] = nullptr;
        }
        prosireniStudenti[vel] = new std::string(ime);
        for(int i = 0; i < vel; i++)
            delete studenti[i];
        delete[] studenti;
        studenti = prosireniStudenti;
        vel+=prosirivac;
    } catch(...) {
        delete[] prosireniStudenti;
        throw std::logic_error("Nedovoljno memorije");
    }
} 

int main() {
    std::string **studenti = nullptr;
    try {
        int vel = 5;
        studenti = new std::string*[5];
        studenti[0] = nullptr;
        studenti[1] = new std::string("Ahmed");
        studenti[2] = nullptr;
        studenti[3] = new std::string("Ahmedov jaran");
        studenti[4] = new std::string("Ahmedova mama"); // sve alokacije manje od n-te neuspjele bi trebalo obrisati
        DodajStudenta(studenti, vel, "Ahmedov babo");
        DodajStudenta(studenti, vel, "Ahmedova zaova");
        DodajStudenta(studenti, vel, "Ahmedov komso");
        DodajStudenta(studenti, vel, "Ahmedov najbolji drug");
        for(int i = 0; i < vel; i++) {
            if(studenti[i] == nullptr) std::cout << "_\n";
            else std::cout << *studenti[i] << "\n";
        }
    } catch(...) {
        std::cout << "Problemi s memorijom!\n";
    }
    return 0;
}