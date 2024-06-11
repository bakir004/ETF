#include <algorithm>
#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class Spremnik {
    double tezina;
    std::string naziv;
public: 
    double DajTezinu() const { return tezina; }
    std::string DajNaziv() const { return naziv; }
    virtual double DajUkupnuTezinu() const = 0;
    virtual void Ispisi() const = 0;
    virtual Spremnik* DajKopiju() const = 0;
    virtual ~Spremnik() { }
    Spremnik(double tezina, std::string naziv): tezina(tezina), naziv(naziv) {}
};

class Sanduk : public Spremnik {
    std::vector<int> tezinePredmeta;
public:
    Sanduk(double tezina, std::string naziv, std::vector<int> tezine): Spremnik(tezina, naziv), tezinePredmeta(tezine) {}
    double DajUkupnuTezinu() const override;
    void Ispisi() const override {
        std::cout << "Vrsta spremnika: Sanduk\n";
        std::cout << "Sadrzaj: " << DajNaziv() << "\n";
        std::cout << "Tezine predmeta: ";
        for(int i = 0; i < tezinePredmeta.size(); i++) std::cout << tezinePredmeta[i] << " ";
        std::cout << "(kg)\nVlastita tezina: " << DajTezinu();
        std::cout << " (kg)\nUkupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
    }
    Spremnik* DajKopiju() const override { return new Sanduk(*this); }
};

double Sanduk::DajUkupnuTezinu() const {
    int tezine = DajTezinu();
    for(int i = 0; i < tezinePredmeta.size(); i++)
        tezine += tezinePredmeta[i];
    return tezine;
}

class Vreca : public Spremnik {
    double tezinaMaterije;
public:
    Vreca(double tezina, std::string naziv, double tezinaMaterije): Spremnik(tezina, naziv), tezinaMaterije(tezinaMaterije) {}
    double DajUkupnuTezinu() const override;
    void Ispisi() const override {
        std::cout << "Vrsta spremnika: Vreca\n";
        std::cout << "Sadrzaj: " << DajNaziv() << "\n";
        std::cout << "Vlastita tezina: " << DajTezinu();
        std::cout << " (kg)\nTezina pohranjene materije: " << tezinaMaterije;
        std::cout << " (kg)\nUkupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
    }
    Spremnik* DajKopiju() const override { return new Vreca(*this); }
};
double Vreca::DajUkupnuTezinu() const {
    return DajTezinu() + tezinaMaterije;
}

class Bure : public Spremnik {
    double tezinaTecnosti;
    int zapremina;
public:
    Bure(double tezina, std::string naziv, double tezinaTecnosti, double zapremina): Spremnik(tezina, naziv), tezinaTecnosti(tezinaTecnosti), zapremina(zapremina) {}
    double DajUkupnuTezinu() const override;
    void Ispisi() const override {
        std::cout << "Vrsta spremnika: Bure\n";
        std::cout << "Sadrzaj: " << DajNaziv() << "\n";
        std::cout << "Vlastita tezina: " << DajTezinu();
        std::cout << " (kg)\nSpecificna tezina tecnosti: " << tezinaTecnosti;
        std::cout << " (kg/m^3)\nZapremina tecnosti: " << zapremina;
        std::cout << " (l)\nUkupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
    }
    Spremnik* DajKopiju() const override { return new Bure(*this); }
};
double Bure::DajUkupnuTezinu() const {
    return DajTezinu() + tezinaTecnosti / 1000 * zapremina;
}

class Skladiste {
    std::vector<std::shared_ptr<Spremnik>> spremnici;
public:
    Skladiste() {}
    Skladiste(const Skladiste& s);
    Skladiste& operator=(const Skladiste& s);
    Spremnik* DodajSanduk(double tezina, std::string naziv, std::vector<int> tezine);
    Spremnik* DodajVrecu(double tezina, std::string naziv, double tezinaMaterije);
    Spremnik* DodajBure(double tezina, std::string naziv, double tezinaTecnosti, double zapremina);
    Spremnik* DodajSpremnik(Spremnik* p, bool predajeVlasnistvo);
    void BrisiSpremnik(Spremnik* p);
    Spremnik& DajNajlaksi() const;
    Spremnik& DajNajtezi() const;
    int BrojPreteskih(int x) const;
    void IzlistajSkladiste() const noexcept;
    void UcitajIzDatoteke(std::string imeDatoteke);
};

void Skladiste::UcitajIzDatoteke(std::string imeDatoteke) {
    std::ifstream ulazniTok(imeDatoteke);
    if(!ulazniTok) throw std::logic_error("Trazena datoteka ne postoji");
    std::vector<std::shared_ptr<Spremnik>> noviSpremnici;
    while(!ulazniTok.eof()) {
        char tip;
        std::string naziv;
        double tezina;
        ulazniTok >> tip;
        if(ulazniTok.eof()) break;
        std::getline(ulazniTok, naziv);
        naziv.erase(0, 1);
        try {
            switch(tip) {
                case 'S': {
                    int n;
                    ulazniTok >> tezina >> n;
                    std::vector<int> tezine(n);
                    for(int i = 0; i < n; i++)
                        ulazniTok >> tezine[i];
                    if(!ulazniTok) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                    noviSpremnici.push_back(std::make_shared<Sanduk>(tezina, naziv, tezine));
                    break;
                } case 'V': {
                    int tezinaMaterije;
                    ulazniTok >> tezina >> tezinaMaterije;
                    if(!ulazniTok) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                    noviSpremnici.push_back(std::make_shared<Vreca>(tezina, naziv, tezinaMaterije));
                    break;
                } case 'B': {
                    int specificnaTezina, zapremina;
                    ulazniTok >> tezina >> specificnaTezina >> zapremina;
                    if(!ulazniTok) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                    noviSpremnici.push_back(std::make_shared<Bure>(tezina, naziv, specificnaTezina, zapremina));
                    break;
                } 
            }
            if(ulazniTok.bad()) throw std::logic_error("Problemi pri citanju datoteke");
        } catch(std::exception& err) {
            noviSpremnici.clear();
            std::cout << err.what();
            throw;
        }
    }
    spremnici = noviSpremnici;
    ulazniTok.close();
}
int Skladiste::BrojPreteskih(int x) const {
    return std::count_if(spremnici.begin(), spremnici.end(), 
        [x](const std::shared_ptr<Spremnik>& s){
            return s->DajUkupnuTezinu() > x;
        });
}
Spremnik& Skladiste::DajNajlaksi() const {
    if(spremnici.empty()) throw std::range_error("Skladiste je prazno");
    return **std::min_element(spremnici.begin(), spremnici.end(), 
        [](const std::shared_ptr<Spremnik>& s1, const std::shared_ptr<Spremnik>& s2){
            return s1->DajUkupnuTezinu() < s2->DajUkupnuTezinu();
            });
}
Spremnik& Skladiste::DajNajtezi() const {
    if(spremnici.empty()) throw std::range_error("Skladiste je prazno");
    return **std::max_element(spremnici.begin(), spremnici.end(), 
        [](const std::shared_ptr<Spremnik>& s1, const std::shared_ptr<Spremnik>& s2){
            return s1->DajUkupnuTezinu() < s2->DajUkupnuTezinu();
            });
}
void Skladiste::BrisiSpremnik(Spremnik* p) {
    // ostavice viseci pokazivac?
    delete p;
}
Spremnik* Skladiste::DodajSpremnik(Spremnik* p, bool predajeVlasnistvo) {
    Spremnik* novaAdresa;
    if(predajeVlasnistvo) {
        spremnici.push_back(std::shared_ptr<Spremnik>(p)); 
        novaAdresa = p;
    } else {
        std::shared_ptr<Spremnik> noviPointer = std::shared_ptr<Spremnik>(p->DajKopiju());
        spremnici.push_back(noviPointer);
        novaAdresa = noviPointer.get();
    } 
    return novaAdresa;
}
void Skladiste::IzlistajSkladiste() const noexcept {
    for(int i = 0; i < spremnici.size(); i++)
        spremnici[i]->Ispisi();
}
Spremnik* Skladiste::DodajBure(double tezina, std::string naziv, double tezinaTecnosti, double zapremina) {
    std::shared_ptr<Spremnik> s = std::make_shared<Bure>(tezina, naziv, tezinaTecnosti, zapremina);
    spremnici.push_back(s);
    return s.get();
}
Spremnik* Skladiste::DodajVrecu(double tezina, std::string naziv, double tezinaMaterije) {
    std::shared_ptr<Spremnik> s = std::make_shared<Vreca>(tezina, naziv, tezinaMaterije);
    spremnici.push_back(s);
    return s.get();
}
Spremnik* Skladiste::DodajSanduk(double tezina, std::string naziv, std::vector<int> tezine) {
    std::shared_ptr<Spremnik> s = std::make_shared<Sanduk>(tezina, naziv, tezine);
    spremnici.push_back(s);
    return s.get();
}

Skladiste::Skladiste(const Skladiste& s) {
    try {
        for(int i = 0; i < s.spremnici.size(); i++) 
            spremnici.push_back(std::shared_ptr<Spremnik>(s.spremnici[i]->DajKopiju()));
    } catch(std::exception& err) {
        spremnici.clear();
        throw;
    }
}
Skladiste& Skladiste::operator=(const Skladiste& s) {
    std::vector<std::shared_ptr<Spremnik>> noviSpremnici;
    try {
        for(int i = 0; i < s.spremnici.size(); i++) 
            noviSpremnici.push_back(std::shared_ptr<Spremnik>(s.spremnici[i]->DajKopiju()));
    } catch(std::exception& err) {
        noviSpremnici.clear();
        throw;
    }
    spremnici = noviSpremnici;
    return *this;
}

int main() {
    // Sanduk* sanduk = new Sanduk(10, "Trofazni kataklingeri za auspuhe", {2,3,1,2,2,4,3,1,3});
    // {
        // Vreca vreca(0.2, "Praskaste cincozne za glajfanje", 5);
        // Bure bure(5, "Rafinirana kalamuta iz Katange", 1300, 150);
    //     Skladiste skladiste;
        // skladiste.DodajSanduk(10, "Trofazni kataklingeri za auspuhe", {2,3,1,2,2,4,3,1,3});
        // skladiste.DodajVrecu(0.2, "Praskaste cincozne za glajfanje", 5);
        // skladiste.DodajBure(5, "Rafinirana kalamuta iz Katange", 1300, 150);
    //     skladiste.DodajSpremnik(sanduk, true);
    //     skladiste.IzlistajSkladiste();
    // }
    // sanduk->Ispisi();
    // skladiste.Ispisi();
    Skladiste s;
    s.DodajSanduk(10, "Trofazni kataklingeri za auspuhe", {2,3,1,2,2,4,3,1,3});
    s.DodajVrecu(0.2, "Praskaste cincozne za glajfanje", 5);
    s.DodajBure(5, "Rafinirana kalamuta iz Katange", 1300, 150);
    // s.IzlistajSkladiste();

    try {
        s.UcitajIzDatoteke("skladiste.txt");
        s.IzlistajSkladiste();
    } catch(std::exception& err) {
        std::cout << err.what();
    }
    return 0;
}
