#include <stdexcept>
#include <iostream>

class Student {
    int indeks, brojPolozenihIspita;
    double prosjek;
public:
    Student() {
        indeks = 0;
        brojPolozenihIspita = 1;
        prosjek = 5;
    }
    int DajIndeks() { return indeks; }
    int DajBrojPolozenih() { return brojPolozenihIspita; }
    double DajProsjek() { return prosjek; }
    void PostaviIndeks(int noviIndeks) {
    if(noviIndeks < 0) throw std::domain_error("Indeks mora biti pozitivan broj!");
    indeks = noviIndeks;
    }
    void RegistrirajOcjenu(int ocjena) {
        if(ocjena == 5)
            return;
        else if(ocjena < 5 || ocjena > 10)
            throw std::domain_error("Ilegalna ocjena!");
        brojPolozenihIspita++;

        prosjek = double(prosjek * (brojPolozenihIspita-1))/double(brojPolozenihIspita) + (double)ocjena/(double)brojPolozenihIspita;
    }
};

int main() {
    Student student;
    std::cout << student.DajProsjek() << "\n";
    student.RegistrirajOcjenu(7);
    std::cout << student.DajProsjek() << "\n";
    return 0;
}
