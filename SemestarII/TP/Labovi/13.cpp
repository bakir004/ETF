#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <array>

enum class GodisnjeDoba {Zima, Proljece, Ljeto, Jesen};
class ApstraktnaBiljka {
    std::string naziv, klima;
    double visina;
    GodisnjeDoba godisnjeDoba;
    bool rijetkost;
public:
    std::array<std::string, 4> godisnjaDoba{{"Zima", "Proljece", "Ljeto", "Jesen"}};
    bool DajRijetkost() const { return rijetkost; }
    GodisnjeDoba DajGodisnjeDoba() const { return godisnjeDoba; }
    double DajVisinu() const { return visina; }
    std::string DajKlimu() const { return klima; }
    std::string DajNaziv() const { return naziv; }
    ApstraktnaBiljka(std::string naziv, std::string klima, double visina, GodisnjeDoba godisnjeDoba, bool rijetkost);
    virtual void Ispisi() const = 0;
    virtual ApstraktnaBiljka* DajKopiju() const = 0;
    virtual ~ApstraktnaBiljka() {}
};
ApstraktnaBiljka::ApstraktnaBiljka(std::string naziv, std::string klima, double visina, GodisnjeDoba godisnjeDoba, bool rijetkost):
    godisnjeDoba(godisnjeDoba), rijetkost(rijetkost) {
        if(naziv.length() == 0 || klima.length() == 0 || visina < 0) throw std::domain_error("Neispravni parametri");
        this->naziv = naziv;
        this->klima = klima;
        this->visina = visina;
    }

class Cvijet : public ApstraktnaBiljka {
    std::string boja;
public:
    std::string DajBoju() const { return boja; }
    Cvijet(std::string naziv, std::string klima, double visina, GodisnjeDoba godisnjeDoba, bool rijetkost, std::string boja):
        ApstraktnaBiljka(naziv, klima, visina, godisnjeDoba, rijetkost) {
            if(boja.length() == 0) throw std::domain_error("Neispravni parametri");
            this->boja = boja;
        }
    void Ispisi() const override {
        std::cout << "Cvijetic boje " << DajBoju() << "\n";
        std::cout << "Naziv: " << DajNaziv() << "\nKlima: " << DajKlimu() << "\nVisina: " << DajVisinu() << "cm\nGodisnje doba: " << godisnjaDoba[(int)DajGodisnjeDoba()] << "\nRijetkost: " << std::boolalpha << DajRijetkost() << "\n\n";
    }
    ApstraktnaBiljka* DajKopiju() const override {
        return new Cvijet(*this);
    }
};

class Trava : public ApstraktnaBiljka {
    std::string ljekovitaSvojstva;
public:
    std::string DajLjekovitaSvojstva() const { return ljekovitaSvojstva; }
    Trava(std::string naziv, std::string klima, double visina, GodisnjeDoba godisnjeDoba, bool rijetkost, std::string ljekovitaSvojstva):
        ApstraktnaBiljka(naziv, klima, visina, godisnjeDoba, rijetkost) {
            if(ljekovitaSvojstva.length() == 0) throw std::domain_error("Neispravni parametri");
            this->ljekovitaSvojstva = ljekovitaSvojstva;
        }
    void Ispisi() const override {
        std::cout << "Trava sa svojstvom " << DajLjekovitaSvojstva() << "\n";
        std::cout << "Naziv: " << DajNaziv() << "\nKlima: " << DajKlimu() << "\nVisina: " << DajVisinu() << "cm\nGodisnje doba: " << godisnjaDoba[(int)DajGodisnjeDoba()] << "\nRijetkost: " << std::boolalpha << DajRijetkost() << "\n\n";
    }
    ApstraktnaBiljka* DajKopiju() const override {
        return new Trava(*this);
    }
};

class Biljka {
    ApstraktnaBiljka *p;
public:
    Biljka(): p(nullptr) {}
    ~Biljka() { delete p; }
    Biljka(const ApstraktnaBiljka& b): p(b.DajKopiju()) {}
    void Ispisi() const { p->Ispisi(); }
};

int main() {
    std::vector<Biljka*> biljke(3, nullptr);
    biljke[0] = new Biljka(Cvijet("Ruza", "Beko", 50, GodisnjeDoba::Proljece, false, "crvena"));
    biljke[1] = new Biljka(Cvijet("Maslacak", "Toshiba", 50, GodisnjeDoba::Proljece, false, "zuta"));
    biljke[2] = new Biljka(Trava("Vutra", "Umjereno kontinentalna", 64, GodisnjeDoba::Ljeto, true, "lijeci stres"));
    for(int i = 0; i < biljke.size(); i++)
        biljke[i]->Ispisi();
    return 0;
}
