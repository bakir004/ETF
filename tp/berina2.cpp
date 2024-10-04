#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class Student {
    std::string ime;
    double prosjek;
public:
    Student(std::string ime, double prosjek): ime(ime), prosjek(prosjek) { }
    std::string DajIme() const { return ime; }
    double DajProsjek() const { return prosjek; }
    void Ispisi() const { std::cout << "Student " << ime << " ima prosjek " << prosjek << "\n"; }
};

// Student** AlocirajStudente(int n) {
//     Student** studenti = nullptr;
//     studenti = new Student*[n]{};
//     try {
//         for(int i = 0; i < n; i++) {
//             studenti[i] = new Student("Bakir", i+1);
//             // if(i == 3) throw std::domain_error("Nema memorije"); // simulacija nestanka memorije
//             // ako nema prostora baca izuzetak bad_alloc
//         }
//     } catch(std::exception& error) {
//         for(int i = 0; i < n; i++) {
//             delete studenti[i];
//         }
//         delete[] studenti;
//         throw;
//     }
//     return studenti;
//     // bad_alloc
// }

std::shared_ptr<Student>* AlocirajStudente(int n) {
    std::shared_ptr<Student>* studenti{};   
    studenti = new std::shared_ptr<Student>[n];
    try {
        for(int i = 0; i < n; i++) {
            // make_shared iza scene koristi new i moze baciti izuzetak kao sto moze new baciti
            // verzija za pametne
            // jedina razlika je sto new vraca obicni pok, a make_shared vraca pametni pok
            studenti[i] = std::make_shared<Student>("Bakir", i+1);
        }
    } catch(...) {
        delete[] studenti;
        throw;
    }

    return studenti;
}

int main() {
    // int* x = new int;
    // int a = 5;
    // int* p = &a;
    // delete x;
    // x = p;
    //
    // int n;
    // std::cin >> n;
    // int* niz = new int[n];
    // for(int i = 0; i < n; i++) {
    //     std::cin >> niz[i];
    // }
    // for(int i = 0; i < n; i++) {
    //     std::cout << niz[i] << " ";
    // }
    // delete[] niz;
    // niz = nullptr;

    // if(niz == nullptr)
    //     std::cout << "Ne pokazuje ni na sta";


    // int* p = nullptr;

    // int* p{};
    // int n = 5;
    //
    // Student** studenti{};
    // try {
    //     studenti = AlocirajStudente(n);
    // } catch(std::exception& error){ // ako AlocirajStudente baci izuzetak
    //     std::cout << error.what();
    // }
    //
    // for(int i = 0; i < n; i++) {
    //     studenti[i]->Ispisi();
    // }
    // for(int i = 0; i < n; i++) {
    //     delete studenti[i];
    // }
    // delete[] studenti;
    //
    // std::shared_ptr<int> pametni(new int(5));
    // std::cout << *pametni;

    // int n = 5;
    // std::shared_ptr<Student>* studenti{};
    // try {
    //     studenti = AlocirajStudente(n);
    // } catch(...) {
    //     std::cout << "GRESKA";
    // }
    // for(int i = 0; i < n; i++) {
    //     studenti[i]->Ispisi();
    // }

    

    return 0;
}





























