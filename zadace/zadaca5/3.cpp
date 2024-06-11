#include <iostream>
#include <memory>
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
    virtual ~Spremnik() {}
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
        std::cout << " (kg)\nVlastita tezina: " << DajTezinu();
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
        std::cout << "Vrsta spremnika: Vreca\n";
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
    Skladiste(const Skladiste& s);
    Skladiste& operator=(const Skladiste& s);
};

int main() {
    Sanduk sanduk(10, "Trofazni kataklingeri za auspuhe", {2,3,1,2,2,4,3,1,3});
    Vreca vreca(0.2, "Praskaste cincozne za glajfanje", 5);
    Bure bure(5, "Rafinirana kalamuta iz Katange", 1300, 150);
    bure.Ispisi();
    return 0;
}
