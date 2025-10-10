#include <cctype>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
const int BrojPredmeta = 8;
struct Datum {
    int dan, mjesec, godina;
};
struct Ucenik {
    std::string ime, prezime;
    Datum datum_rodjenja;
    int ocjene[BrojPredmeta];
    double prosjek;
    bool prolaz;
};
int main() {
    void UnesiUcenike(std::vector<Ucenik> &ucenici);
    void ObradiUcenike(std::vector<Ucenik> &ucenici);
    void IspisiIzvjestaj(const std::vector<Ucenik> &ucenici);
    int broj_ucenika;
    std::cout << "Koliko ima ucenika: ";
    std::cin >> broj_ucenika;
    try {
        std::vector<Ucenik> ucenici(broj_ucenika);
        UnesiUcenike(ucenici);
        ObradiUcenike(ucenici);
        IspisiIzvjestaj(ucenici);
    }
    catch(...) {
        std::cout << "Problemi sa memorijom...\n";
    }
    return 0;
}
void UnesiUcenike(std::vector<Ucenik> &ucenici) {
    void UnesiJednogUcenika(Ucenik &ucenik);
    for(int i = 0; i < ucenici.size(); i++) {
        std::cout << "Unesite podatke za " << i + 1 << ". ucenika:\n";
        UnesiJednogUcenika(ucenici[i]);
    }
}
void UnesiJednogUcenika(Ucenik &ucenik) {
    void UnesiDatum(Datum &datum);
    void UnesiOcjene(int ocjene[], int broj_predmeta);
    while(true) {
        std::cout << " Ime: "; std::cin >> ucenik.ime;
        if(std::count_if(ucenik.ime.begin(), ucenik.ime.end(), [](char c){return !std::isalpha(c);}) > 0)
            std::cout << "Neispravno ime, ponovite unos.\n";
        else break;
    }
    while(true) {
        std::cout << " Prezime: "; std::cin >> ucenik.ime;
        if(std::count_if(ucenik.prezime.begin(), ucenik.prezime.end(), [](char c){return !std::isalpha(c);}) > 0)
            std::cout << "Neispravno prezime, ponovite unos.\n";
        else break;
    }
    std::cout << " Datum rodjenja (D/M/G): ";
    UnesiDatum(ucenik.datum_rodjenja);
    UnesiOcjene(ucenik.ocjene, BrojPredmeta);
}
void UnesiDatum(Datum &datum) {
    char znak;
    while(true) {
        std::cin >> datum.dan >> znak >> datum.mjesec >> znak >> datum.godina;
        int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if(datum.godina % 4 == 0 && datum.godina % 100 != 0 || datum.godina % 400 == 0) broj_dana[1]++;
        if(datum.godina < 1 || datum.mjesec < 1 || datum.mjesec > 12 || datum.dan < 1 || datum.dan > broj_dana[datum.mjesec - 1])
            std::cout << "Neispravan datum, ponovite unos.\n";
        else break;
    }
}
void UnesiOcjene(int ocjene[], int broj_predmeta) {
    void UnesiOcjenu(int &ocjena);
    for(int i = 0; i < broj_predmeta; i++) {
        std::cout << " Ocjena iz " << i + 1 << ". predmeta: ";
        UnesiOcjenu(ocjene[i]);
    }
}
void UnesiOcjenu(int &ocjena) {
    while(true) {
        std::cin >> ocjena;
        if(ocjena < 5 || ocjena > 10 || !(std::cin)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Neispravna ocjena, ponovite unos.\n";
        }
        else break;
    }

}
void ObradiUcenike(std::vector<Ucenik> &ucenici) {
    void ObradiJednogUcenika(Ucenik &ucenik);
    for(Ucenik &ucenik : ucenici) ObradiJednogUcenika(ucenik);
    std::sort(ucenici.begin(), ucenici.end(), [](const Ucenik &u1, const Ucenik &u2) {
            return u1.prosjek > u2.prosjek;
            });
}
void ObradiJednogUcenika(Ucenik &ucenik) {
    double suma_ocjena = 0;
    ucenik.prosjek = 1; ucenik.prolaz = false;
    for(int ocjena : ucenik.ocjene) {
        if(ocjena == 1) return;
        suma_ocjena += ocjena;
    }
    ucenik.prolaz = true;
    ucenik.prosjek = suma_ocjena / BrojPredmeta;
}
void IspisiIzvjestaj(const std::vector<Ucenik> &ucenici) {
    void IspisiJednogUcenika(const Ucenik &ucenik);
    std::cout << std::endl;
    for(const Ucenik &ucenik : ucenici) IspisiJednogUcenika(ucenik);
}
void IspisiJednogUcenika(const Ucenik &ucenik) {
    void IspisiDatum(const Datum &datum);
    std::cout << "Ucenik " << ucenik.ime << " " << ucenik.prezime << " rodjen ";
    IspisiDatum(ucenik.datum_rodjenja);
    if(ucenik.prolaz)
        std::cout << " ima prosjek " << std::setprecision(3) << ucenik.prosjek;
    else
        std::cout << " mora ponavljati razred";
    std::cout << std::endl;
}
void IspisiDatum(const Datum &datum) {
    std::cout << datum.dan << "/" << datum.mjesec << "/" << datum.godina;
}

