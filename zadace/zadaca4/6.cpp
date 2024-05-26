#include <algorithm>
#include <map>
#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <string>

class Student {
    int brojIndeksa;
    std::string ime, adresa, godina, brojTelefona;
    std::vector<std::string> validneGodine{"1","2","3","1/B","2/B","3/B","1/M","2/M","1/D","2/D","3/D"};
    std::map<std::string, std::string> mapaGodina{{"1", "1/B"},{"2","2/B"},{"3","3/B"}};
    void TestIndexa(int brojIndeksa) const;
    void TestGodine(std::string godina) const;
    void TestBrojaTelefona(std::string brojTelefona) const;
    void OcistiIme(std::string &ime) const;
public:
    Student(int brojIndeksa, std::string ime, std::string adresa, std::string godina, std::string brojTelefona);
    int DajIndeks() { return brojIndeksa; }
    std::string DajGodinuStudija() { 
        if(mapaGodina.find(godina) != mapaGodina.end()) return mapaGodina[godina];
        else return godina; 
    }
    std::string DajImePrezime() { return ime; }
    std::string DajAdresu() { return adresa; }
    std::string DajTelefon() { return brojTelefona; }
    void Ispisi();
};

void Student::OcistiIme(std::string &ime) const {
    std::string preciscen = "";
    bool bioRazmak = true;
    for(int i = 0; i < ime.length(); i++) {
        if(std::isalpha(ime[i])) {
            preciscen += ime[i];
            bioRazmak = false;
        } else if(ime[i] == ' ' && bioRazmak == false) {
            preciscen += ' ';
            bioRazmak = true;
        }
    }
    if(preciscen[preciscen.length()-1] == ' ') preciscen.pop_back();
    ime = preciscen;
}
void Student::Ispisi() {
    std::cout << "Broj indeksa: " << brojIndeksa << "\n";
    std::cout << "Godina studija: " << DajGodinuStudija() << "\n";
    std::cout << "Ime i prezime: " << ime << "\n";
    std::cout << "Adresa: " << adresa << "\n";
    std::cout << "Broj telefona: " << brojTelefona << "\n";
}
Student::Student(int brojIndeksa, std::string ime, std::string adresa, std::string godina, std::string brojTelefona): adresa(adresa)  {
    OcistiIme(ime);
    TestIndexa(brojIndeksa);
    TestGodine(godina);
    TestBrojaTelefona(brojTelefona);
    this->ime = ime;
    this->brojIndeksa = brojIndeksa;
    this->godina = godina;
    this->brojTelefona = brojTelefona;
}
void Student::TestIndexa(int brojIndeksa) const {
    if(brojIndeksa < 10000 || brojIndeksa > 99999)
        throw std::domain_error("Neispravni parametri");
}
void Student::TestGodine(std::string godina) const {
    if(std::find(validneGodine.begin(), validneGodine.end(), godina) == validneGodine.end())
        throw std::domain_error("Neispravni parametri");
}
void Student::TestBrojaTelefona(std::string brojTelefona) const {
    int i = 0;
    if(i == brojTelefona.length() || !std::isdigit(brojTelefona[i]))
        throw std::domain_error("Neispravni parametri");

    while(++i < brojTelefona.length() && std::isdigit(brojTelefona[i])) {}

    if(i == brojTelefona.length() || brojTelefona[i] != '/')
        throw std::domain_error("Neispravni parametri");
    if(++i == brojTelefona.length() && !std::isdigit(brojTelefona[i]))
        throw std::domain_error("Neispravni parametri");

    while(++i < brojTelefona.length() && std::isdigit(brojTelefona[i])) {}

    if(i == brojTelefona.length() || brojTelefona[i] != '-')
        throw std::domain_error("Neispravni parametri");
    if(++i == brojTelefona.length() || !std::isdigit(brojTelefona[i]))
        throw std::domain_error("Neispravni parametri");

    while(++i < brojTelefona.length() && std::isdigit(brojTelefona[i])) {}

    if(i < brojTelefona.length())
        throw std::domain_error("Neispravni parametri");
}

// noexcept

class Laptop {
    int ev_broj;
    std::string naziv, karakteristike;
    std::shared_ptr<Student> kod_koga_je;
public:
    int DajEvidencijskiBroj() const { return ev_broj; }
    std::string DajNaziv() const { return naziv; }
    std::string DajKarakteristike() const { return karakteristike; }
    Laptop(int broj, std::string naziv, std::string karakteristike): naziv(naziv), karakteristike(karakteristike) {
        if(broj < 0) throw std::domain_error("Neispravni parametri");
        kod_koga_je = nullptr;
    }
    void Zaduzi(Student &s);
    void Razduzi() { kod_koga_je = nullptr; }
    bool DaLiJeZaduzen() const { return kod_koga_je != nullptr; }
    Student &DajKodKogaJe() const;
    std::shared_ptr<Student> DajPokKodKogaJe() const noexcept { return kod_koga_je; }
    void Ispisi() const;
};

void Laptop::Ispisi() const {
    std::cout << "Evidencijski broj: " << ev_broj << "\n";
    std::cout << "Naziv: " << naziv << "\n";
    std::cout << "Karakteristike: " << karakteristike << "\n";
}

Student& Laptop::DajKodKogaJe() const {
    if(kod_koga_je == nullptr)
        throw std::domain_error("Laptop nije zaduzen");
    return *kod_koga_je;
}
void Laptop::Zaduzi(Student &s) {
    if(kod_koga_je != nullptr) throw std::domain_error("Laptop vec zaduzen");
    kod_koga_je = std::make_shared<Student>(s);
}

class Administracija {
    std::map<int, std::shared_ptr<Student>> studenti;
    std::map<int, std::shared_ptr<Laptop>> laptopi;
public:
    Administracija() {}
    Administracija(const Administracija& admin);
    Administracija(Administracija&& admin);
    Administracija& operator=(const Administracija& admin);
    Administracija& operator=(Administracija&& admin);
    void RegistrirajNovogStudenta(int brojIndeksa, std::string ime, std::string adresa, std::string godina, std::string brojTelefona);
    void RegistrirajNoviLaptop(int broj, std::string naziv, std::string karakteristike);
    Student& NadjiStudenta(int brojIndeksa) const;
    Student NadjiStudenta(int brojIndeksa);
    Laptop& NadjiLaptop(int evidencijskiBroj) const;
    Laptop NadjiLaptop(int evidencijskiBroj);
    void IzlistajStudente() const;
    void IzlistajLaptope() const;
    void ZaduziLaptop(int brojIndeksa, int evidencijskiBroj);
    int NadjiSlobodniLaptop() const;
    void RazduziLaptop(int evidencijskiBroj);
    void PrikaziZaduzenja() const;
    ~Administracija();
};

Administracija& Administracija::operator=(Administracija&& admin) {
    for(auto &par : studenti) par.second = nullptr;
    for(auto &par : laptopi) par.second = nullptr;
    studenti = admin.studenti;
    laptopi = admin.laptopi;
    for(auto par : admin.studenti)
        par.second = nullptr;
    for(auto par : admin.laptopi)
        par.second = nullptr;
    return *this;
}

Administracija& Administracija::operator=(const Administracija& admin) {
    std::map<int, std::shared_ptr<Student>> noviStudenti;
    std::map<int, std::shared_ptr<Laptop>> noviLaptopi;
    try {
        for(auto par : admin.studenti) noviStudenti[par.first] = std::make_shared<Student>(*par.second);
        for(auto par : admin.laptopi) noviLaptopi[par.first] = std::make_shared<Laptop>(*par.second);
        for(auto par : studenti) par.second = nullptr;
        for(auto par : laptopi) par.second = nullptr;
        studenti = noviStudenti;
        laptopi = noviLaptopi;
    } catch(...) {
        for(auto par : noviStudenti) par.second = nullptr;
        for(auto par : noviLaptopi) par.second = nullptr;
        throw;
    }
    return *this;
}

Administracija::Administracija(Administracija&& admin) {
    studenti = admin.studenti;
    laptopi = admin.laptopi;
    for(auto par : admin.studenti)
        par.second = nullptr;
    for(auto par : admin.laptopi)
        par.second = nullptr;
}

Administracija::Administracija(const Administracija& admin) {
    try {
        for(auto par : admin.studenti) studenti[par.first] = std::make_shared<Student>(*par.second);
        for(auto par : admin.laptopi) laptopi[par.first] = std::make_shared<Laptop>(*par.second);
    } catch(...) {
        for(auto par : studenti) par.second = nullptr;
        for(auto par : laptopi) par.second = nullptr; 
        throw;
    }
}

Administracija::~Administracija() {
    int counter = 0;
    for(auto par : studenti) {
        par.second = nullptr; 
        counter++;
    }
    for(auto par : laptopi) { 
        par.second = nullptr; 
        counter++;
    }
    std::cout << "Uspjesno obrisano " << counter << " objekata!\n";
}
void Administracija::PrikaziZaduzenja() const {
    bool imaMakarJedanZaduzen = false;
    for(auto par : laptopi) {
        std::shared_ptr<Laptop> laptop = par.second;
        std::shared_ptr<Student> zaduzeniStudent = laptop->DajPokKodKogaJe();
        if(zaduzeniStudent == nullptr) continue;
        imaMakarJedanZaduzen = true;
        std::cout << "Student " << zaduzeniStudent->DajImePrezime() << " zaduzio/la laptop broj " << par.first << "\n";
    }
    if(!imaMakarJedanZaduzen)
        std::cout << "Nema zaduzenja\n";
    std::cout << "\n";
}
void Administracija::RazduziLaptop(int evidencijskiBroj) {
    if(laptopi.find(evidencijskiBroj) == laptopi.end()) throw std::domain_error("Laptop nije nadjen");
    if(!laptopi[evidencijskiBroj]->DaLiJeZaduzen()) throw std::domain_error("Laptop nije zaduzen");
    laptopi[evidencijskiBroj]->Razduzi();
}
int Administracija::NadjiSlobodniLaptop() const {
    for(auto par : laptopi)
        if(!par.second->DaLiJeZaduzen())
            return par.first;
    throw std::domain_error("Nema slobodnih laptopa");
}

void Administracija::ZaduziLaptop(int brojIndeksa, int evidencijskiBroj) {
    if(studenti.find(brojIndeksa) == studenti.end()) throw std::domain_error("Student nije nadjen");
    if(laptopi.find(evidencijskiBroj) == laptopi.end()) throw std::domain_error("Laptop nije nadjen");
    if(laptopi[evidencijskiBroj]->DaLiJeZaduzen()) throw std::domain_error("Laptop vec zaduzen");
    for(auto par : laptopi) {
        Laptop laptop = *(par.second);
        std::shared_ptr<Student> studentKodKogaJe = laptop.DajPokKodKogaJe();
        if(studentKodKogaJe == nullptr) continue;
        if(studentKodKogaJe->DajIndeks() == brojIndeksa)
            throw std::domain_error("Student je vec zaduzio laptop");
    }
    laptopi[evidencijskiBroj]->Zaduzi(*studenti[brojIndeksa]);
}
void Administracija::IzlistajLaptope() const {
    for(auto par : laptopi) {
        par.second->Ispisi();
        if(par.second->DaLiJeZaduzen()) {
            std::shared_ptr<Student> s = par.second->DajPokKodKogaJe();
            std::cout << "Zaduzio(la): " << s->DajImePrezime() << " (" << s->DajIndeks() << ")";
        }
        std::cout << "\n";
    }
}
void Administracija::IzlistajStudente() const {
    for(auto par : studenti) {
        par.second->Ispisi();
        std::cout << '\n';
    }
}
Laptop& Administracija::NadjiLaptop(int evidencijskiBroj) const {
    auto it = laptopi.find(evidencijskiBroj); 
    if(it == laptopi.end())
        throw std::domain_error("Laptop nije nadjen");
    return *(it->second);
}
Laptop Administracija::NadjiLaptop(int evidencijskiBroj) {
    if(laptopi.find(evidencijskiBroj) == laptopi.end())
        throw std::domain_error("Laptop nije nadjen");
    return *laptopi[evidencijskiBroj];
}
Student& Administracija::NadjiStudenta(int brojIndeksa) const {
    auto it = studenti.find(brojIndeksa); 
    if(it == studenti.end())
        throw std::domain_error("Student nije nadjen");
    return *(it->second);
}

Student Administracija::NadjiStudenta(int brojIndeksa) {
    if(studenti.find(brojIndeksa) == studenti.end())
        throw std::domain_error("Student nije nadjen");
    return *studenti[brojIndeksa];
}

void Administracija::RegistrirajNoviLaptop(int broj, std::string naziv, std::string karakteristike) {
    if(laptopi.find(broj) != laptopi.end())
        throw std::domain_error("Laptop s tim evidencijskim brojem vec postoji");
    laptopi[broj] = std::make_shared<Laptop>(broj, naziv, karakteristike);
}

void Administracija::RegistrirajNovogStudenta(int brojIndeksa, std::string ime, std::string adresa, std::string godina, std::string brojTelefona) {
    if(studenti.find(brojIndeksa) != studenti.end())
        throw std::domain_error("Student s tim indeksom vec postoji");
    studenti[brojIndeksa] = std::make_shared<Student>(brojIndeksa, ime, adresa, godina, brojTelefona);
}

int main() {
    Administracija admin1;
    admin1.RegistrirajNovogStudenta(19705, "Bakir Cinjarevic", "Celjigovici 47", "3/D", "061/670-705");
    admin1.RegistrirajNovogStudenta(19706, "    Zakir   Cinjarevic   ", "Celjigovici 47", "2/D", "062/290-305");
    admin1.RegistrirajNoviLaptop(69, "ASUS", "100 na sat");
    admin1.RegistrirajNoviLaptop(70, "HP", "VRMMMMMMMMMMM");
    admin1.ZaduziLaptop(19705, 69);
    admin1.ZaduziLaptop(19706, 70);
    admin1.PrikaziZaduzenja();
    Administracija admin2 = admin1;
    admin2.RegistrirajNovogStudenta(19707, "    Nigga Cinjarevic   ", "Celjigovici 47", "1", "062/290-305");
    admin2.RazduziLaptop(69);
    admin2.ZaduziLaptop(19707, 69);
    admin2.RazduziLaptop(70);
    admin2.PrikaziZaduzenja();
    return 0;
}
