#include <iostream>
#include <stdexcept>
#include <string>

void ObrisiStudenta(std::string **&studenti, int &vel, std::string ime) {
    int brojNulPokazivaca = 0;
    for(int i = 0; i < vel; i++) {
        if(studenti[i] != nullptr && *studenti[i] == ime) {
            delete studenti[i];
            studenti[i] = nullptr;
        }
        if(studenti[i] == nullptr)
            brojNulPokazivaca++;
    }
    if(brojNulPokazivaca <= vel/2)
        return;

    std::string **noviStudenti = nullptr;
    try {
        noviStudenti = new std::string*[vel-brojNulPokazivaca]{};
        int index = 0;
        for(int i = 0; i < vel; i++) {
            if(studenti[i] != nullptr) {
                noviStudenti[index] = studenti[i];
                index++;
            }
        }
        for(int i = 0; i < vel; i++)
            delete studenti[i];
        delete[] studenti;
        studenti = noviStudenti;
        vel -= brojNulPokazivaca;
    } catch(...) {
        delete[] noviStudenti;
        throw std::logic_error("Nedovoljno memorije");
    }
}

int main() {
    std::string **studenti = nullptr;
    try {
        int vel = 6;
        studenti = new std::string*[vel];
        studenti[0] = nullptr;
        studenti[1] = new std::string("Ahmed");
        studenti[2] = new std::string("Ahmeda");
        studenti[3] = new std::string("Ahmedov jaran");
        studenti[4] = new std::string("Ahmedova mama");
        studenti[5] = new std::string("Ahmedov babo");
        ObrisiStudenta(studenti, vel, std::string("Ahmedov babo"));
        ObrisiStudenta(studenti, vel, std::string("Ahmedov jaran"));
        ObrisiStudenta(studenti, vel, std::string("Ahmed"));
        for(int i = 0; i < vel; i++) {
            if(studenti[i] == nullptr) std::cout << "_\n";
            else std::cout << *studenti[i] << "\n";
        }
    } catch(...) {
        std::cout << "Problemi s memorijom!\n";
    }
    return 0;
}