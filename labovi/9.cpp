#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Datum {
    int dan, mjesec, godina;
    std::vector<std::string> naziviMjeseci{"Januar", "Februar", "Mart", "April", "Maj", "Juni", "Juli", "August", "Septembar", "Oktobar", "Novembar", "Decembar"};
    std::vector<std::string> naziviDana{"Ponedjeljak", "Utorak", "Srijeda", "Cetvrtak", "Petak", "Subota", "Nedjelja"};
    int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int DatumUDane() const {
        int dani = 0;
        for(int i = 1; i <= godina-1; i++) {
            if(PrestupnaGodina(godina)) dani += 366;
            else dani += 365;
        }
        for(int i = 0; i < mjesec-1; i++) {
            if(i == 1 && PrestupnaGodina(godina))
                dani += 29;
            else 
                dani += broj_dana[i];
        }
        dani += dan;
        return dani;
    }
public:
    // konstruktori
    Datum(): dan(1), mjesec(1), godina(1) {}
    Datum(int dan, int mjesec, int godina) {
        try {
            PostaviDatum(dan, mjesec, godina);
        } catch(std::domain_error) {
            throw std::logic_error("Parametri nisu u dozvoljenom opsegu!\n");
        }
    }
    explicit Datum(int brojDana) {
        if(brojDana < 0) throw std::logic_error("Parametar ne smije biti negativan!\n");
        dan = 1;
        mjesec = 1;
        godina = 1;
        this->PomjeriDatumZa(brojDana);
    }
    Datum(int brojDana, int godina) {
        if(brojDana < 0 || brojDana >= 365 + this->PrestupnaGodina(godina))
            throw std::logic_error("Broj dana mora biti u opsegu 0-365 (366 ako prestupna)!\n");
        dan = 1;
        mjesec = 1;
        this->godina = godina;
        this->PomjeriDatumZa(brojDana);
    }
    Datum(const Datum &drugi) {
        dan = drugi.DajDan();
        mjesec = drugi.DajMjesec();
        godina = drugi.DajGodinu();
    }
    // end konstruktori

    void PostaviDatum(int dan, int mjesec, int godina) {
        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0)
            broj_dana[1]++;
        if(godina < 1 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > broj_dana[mjesec - 1])
            throw std::domain_error("Neispravan datum!");
        this->dan = dan; this->mjesec = mjesec; this->godina = godina;
    }
    void PostaviDan(int dan) {
        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;
        if(dan < 1 || dan > broj_dana[this->mjesec - 1])
            throw std::domain_error("Neispravan dan!");
        this->dan = dan;
    }
    void PostaviMjesec(int mjesec) {
        if(mjesec < 1 || mjesec > 12)
            throw std::domain_error("Neispravan mjesec!");
        this->mjesec = mjesec;
    }
    void PostaviGodinu(int godina) {
        this->godina = godina;
    }
    void PomjeriDatumZa(int brojDana) {
        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0)
            broj_dana[1]++;
        if(brojDana > 0) {
            for(int i = 0; i < brojDana; i++) {
                dan++;
                if(dan > broj_dana[mjesec - 1]) dan = 1, mjesec++;
                if(mjesec > 12) mjesec = 1, godina++;
            }
        } else {
            for(int i = 0; i < -brojDana; i++) {
                dan--;
                if(dan == 0) mjesec--;
                if(mjesec == 0) {
                    godina--;
                    mjesec = 12;
                    dan = 31;
                } else dan = broj_dana[mjesec-1];
            }
        }
    }
    bool PrestupnaGodina(int godina) const {
        return godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0;
    }
    int DajDan() const { return dan; }
    int DajMjesec() const { return mjesec; }
    int DajGodinu() const { return godina; }
    std::string DajNazivMjeseca() const {
        return naziviMjeseci[mjesec-1];
    }
    void IspisiDatum() const {
        std::cout << std::setw(2) << std::setfill('0') << dan << "." << std::setw(2) << std::setfill('0') << mjesec << "." << std::setw(4) << std::setfill('0') << godina;
    }
    void IspisiDatumSNazivom() const {
        std::cout << std::setw(2) << std::setfill('0') << dan << ". " << DajNazivMjeseca() << " " << std::setw(4) << std::setfill('0') << godina << ".\n";
    }
    bool VeciOd(const Datum &drugi) const {
        if(godina == drugi.DajGodinu()) {
            if(mjesec == drugi.DajMjesec())
                return dan > drugi.DajDan();
            return mjesec > drugi.DajMjesec();
        }
        return godina > drugi.DajGodinu();
    }
    bool ManjiOd(const Datum &drugi) const {
        if(godina == drugi.DajGodinu()) {
            if(mjesec == drugi.DajMjesec())
                return dan < drugi.DajDan();
            return mjesec < drugi.DajMjesec();
        }
        return godina < drugi.DajGodinu();
    }
    bool JednakSa(const Datum &drugi) const {
        return dan == drugi.DajDan() && mjesec == drugi.DajMjesec() && godina == drugi.DajGodinu();
    }
    void SaberiSa(const Datum &d) {
        dan += d.dan;
        mjesec += d.mjesec;
        godina += d.godina;
        if(mjesec > 12) godina++, mjesec-=12;
        if(dan > broj_dana[mjesec-1]) mjesec++, dan-=broj_dana[mjesec-1];
    }
    std::string DajDanUSedmici() const {
        int dani = DatumUDane();
        return naziviDana[(dani-1)%7];
    }
    friend bool VeciDatum(const Datum &d1, const Datum &d2);
    friend bool JednakiDatumi(const Datum &d1, const Datum &d2);
    friend void SaberiDatume(Datum &d, int brojDana);
    friend int ProtekloDana(const Datum &d1, const Datum &d2);
};

bool VeciDatum(const Datum &d1, const Datum &d2) {
    if(d1.godina == d2.godina) {
        if(d1.mjesec == d2.mjesec)
            return d1.dan > d2.dan;
        return d1.mjesec > d2.mjesec;
    }
    return d1.godina > d2.godina;
}

bool JednakiDatumi(const Datum &d1, const Datum &d2) {
    return d1.dan == d2.dan && d1.mjesec == d2.mjesec && d1.godina == d2.godina;
}
int ProtekloDana(const Datum &d1, const Datum &d2) {
    return d1.DatumUDane() - d2.DatumUDane();
}

void SaberiDatume(Datum &d, int brojDana) {
    if(d.godina % 4 == 0 && d.godina % 100 != 0 || d.godina % 400 == 0)
        d.broj_dana[1]++;
    if(brojDana > 0) {
        for(int i = 0; i < brojDana; i++) {
            d.dan++;
            if(d.dan > d.broj_dana[d.mjesec - 1]) d.dan = 1, d.mjesec++;
            if(d.mjesec > 12) d.mjesec = 1, d.godina++;
        }
    } else {
        for(int i = 0; i < -brojDana; i++) {
            d.dan--;
            if(d.dan == 0) d.mjesec--;
            if(d.mjesec == 0) {
                d.godina--;
                d.mjesec = 12;
                d.dan = 31;
            } else d.dan = d.broj_dana[d.mjesec-1];
        }
    }
}

class Praznici {
    int ukupno, poMjesecima[12]{};
    Datum **datumi;
public:
    explicit Praznici(int maks): ukupno(maks), datumi(new Datum*[ukupno]{}) {
        std::fill(std::begin(poMjesecima), std::end(poMjesecima), 0);
    };
    Praznici(std::vector<Datum> datumi): ukupno(datumi.size()), datumi(new Datum*[ukupno]{}) {
        try {
            for(int i = 0; i < ukupno; i++) {
                this->datumi[i] = new Datum(datumi[i]);
                this->poMjesecima[datumi[i].DajMjesec()-1]++;
            }
        } catch(...) {
            for(int i = 0; i < ukupno; i++)
                delete this->datumi[i];
            delete[] this->datumi;
            std::fill(std::begin(poMjesecima), std::end(poMjesecima), 0);
            throw;
        }
    }
    Praznici(const Praznici &p) {
        ukupno = p.ukupno;
        std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
        Datum **noviDatumi = new Datum*[ukupno]{};
        try {
            for(int i = 0; i < ukupno; i++) {
                if(p.datumi[i] == nullptr)
                    noviDatumi[i] = nullptr;
                else
                    noviDatumi[i] = new Datum(*(p.datumi[i]));
            }
        } catch(...) {
            for(int i = 0; i < ukupno; i++)
                delete noviDatumi[i];
            delete[] noviDatumi;
            throw;
        }
        datumi = noviDatumi;
    }
    Praznici(Praznici &&p) {
        if(ukupno != p.ukupno) throw std::domain_error("Nesaglasne dimenzije");
        std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
        datumi = p.datumi;
        p.datumi = nullptr;
        p.ukupno = 0;
    }
    Praznici &operator =(const Praznici &p) {
        if(ukupno != p.ukupno) throw std::domain_error("Nasaglasne dimenzije");
        // brisanje postojeceg
        for(int i = 0; i < ukupno; i++)
            delete datumi[i];
        delete[] datumi;

        // duboko kopiranje iz novog
        ukupno = p.ukupno;
        std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
        Datum **noviDatumi = new Datum*[ukupno]{};
        try {
            for(int i = 0; i < ukupno; i++)
                noviDatumi[i] = new Datum(*(p.datumi[i]));
        } catch(...) {
            for(int i = 0; i < ukupno; i++)
                delete noviDatumi[i];
            delete[] noviDatumi;
            throw;
        }
        datumi = noviDatumi;
        return *this;
    }
    Praznici &operator =(Praznici &&p) {
        if(ukupno != p.ukupno) throw std::domain_error("Nesaglasne dimenzije");
        // brisanje postojeceg
        for(int i = 0; i < ukupno; i++)
            delete datumi[i];
        delete[] datumi;

        // kradja
        ukupno = p.ukupno;
        std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
        datumi = p.datumi;
        p.datumi = nullptr;
        return *this;
    }
    void DodajPraznik(Datum d) {
        if(Praznik(d)) throw std::logic_error("Postoji praznik");
        int brojPraznika = 0;
        for(int i = 0; i < 12; i++)
            brojPraznika+=poMjesecima[i];
        if(ukupno == brojPraznika) throw std::logic_error("Nema dovoljno mjesta");

        for(int i = 0; i < ukupno; i++)
            if(datumi[i] == nullptr) {
                datumi[i] = new Datum(d);
                break;
            }
        poMjesecima[d.DajMjesec()-1]++;
    }
    bool Praznik(Datum d) {
        for(int i = 0; i < ukupno; i++)
            if(datumi[i] != nullptr && JednakiDatumi(d, *(datumi[i])))
                return true;
        return false;
    }
    void IspisiPraznike();
    void IspisiPraznikeMjeseca(int m);
    ~Praznici() {
        for(int i = 0; i < ukupno; i++)
            delete datumi[i];
        delete[] datumi;
    };
};

void Praznici::IspisiPraznike() {
    std::vector<Datum*> vektorDatuma;
    for(int i = 0; i < ukupno; i++)
        if(datumi[i] != nullptr)
            vektorDatuma.push_back(datumi[i]);
    sort(vektorDatuma.begin(), vektorDatuma.end(), [](Datum* d1, Datum* d2){
            return !VeciDatum(*d1, *d2);
            });
    for(int i = 0; i < vektorDatuma.size(); i++) {
        vektorDatuma[i]->IspisiDatum();
        std::cout << ", " << vektorDatuma[i]->DajDanUSedmici() << "\n";
    }
    std::cout << "\n";
}
void Praznici::IspisiPraznikeMjeseca(int m) {
    std::vector<Datum*> vektorDatuma;
    for(int i = 0; i < ukupno; i++)
        if(datumi[i] != nullptr && datumi[i]->DajMjesec() == m)
            vektorDatuma.push_back(datumi[i]);

    sort(vektorDatuma.begin(), vektorDatuma.end(), [](Datum* d1, Datum* d2){
            return !VeciDatum(*d1, *d2);
            });
    for(int i = 0; i < vektorDatuma.size(); i++) {
        vektorDatuma[i]->IspisiDatum();
        std::cout << ", " << vektorDatuma[i]->DajDanUSedmici() << "\n";
    }
    std::cout << "\n";
}

int main() {
    // Praznici p1({Datum(1,1,2004), Datum(3,3,2024), Datum(5,3,2004)});
    // p1.DodajPraznik(Datum(1,2,2004));
    // p.IspisiPraznike();
    // p1.IspisiPraznikeMjeseca(3);
    Praznici p2(3);
    p2.DodajPraznik(Datum(27,2,2004));
    p2.DodajPraznik(Datum(29,3,2004));
    p2.IspisiPraznikeMjeseca(2);
    Praznici p3 = p2; // kopirajuci konstruktor
    p3.DodajPraznik(Datum(23,5,2024));
    p3.IspisiPraznike();
    // Datum d1, d2;
    // d1.PostaviDatum(9, 5, 2024);
    // d2.PostaviDatum(9, 5, 2024);
    //
    // d1.IspisiDatumSNazivom();
    // d2.IspisiDatumSNazivom();
    // std::cout << "\nDan u sedmici datuma ";
    // d2.IspisiDatumSNazivom();
    // std::cout << "je " << d2.DajDanUSedmici() << "\n\n";
    // std::cout << "Jednaki d1 i d2 - " << std::boolalpha << JednakiDatumi(d1, d2) << "\n";
    // d2.PomjeriDatumZa(5);
    // std::cout << "Pomjeramo za 5 dana: ";
    // d2.IspisiDatumSNazivom();
    // std::cout << "Njihova razlika je: " << ProtekloDana(d1, d2) << "\n";
    // Datum d1;
    // d1.IspisiDatum();
    // Datum d2(27,2,2004);
    // d2.IspisiDatum();
    // try {
    //     Datum d3(29,2,1900);
    // } catch(std::logic_error err) {
    //     std::cout << err.what();
    // }
    // Datum d4(60);
    // d4.IspisiDatum();
    // Datum d5(60, 2000);
    // d5.IspisiDatum();
    // try {
    //     Datum d5(367, 2000);
    //     d5.IspisiDatum();
    // } catch(std::logic_error err) {
    //     std::cout << err.what();
    // }
    // Datum d6(d2);
    // Datum d7 = d6;
    // d6.IspisiDatum();
    // d7.IspisiDatum();
    return 0;
}
