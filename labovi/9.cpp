#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
// dodo komentar
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
        std::cout << std::setw(2) << std::setfill('0') << dan << "." << std::setw(2) << std::setfill('0') << mjesec << "." << std::setw(4) << std::setfill('0') << godina << ".\n";
    }
    void IspisiDatumSNazivom() const {
        std::cout << std::setw(2) << std::setfill('0') << dan << ". " << DajNazivMjeseca() << " " << std::setw(4) << std::setfill('0') << godina << ".\n";
    }
    bool VeciOd(const Datum &drugi) const {
        if(godina == drugi.godina) {
            if(mjesec == drugi.mjesec)
                return dan > drugi.dan;
            return mjesec > drugi.mjesec;
        }
        return godina > drugi.godina;
    }
    bool ManjiOd(const Datum &drugi) const {
        if(godina == drugi.godina) {
            if(mjesec == drugi.mjesec)
                return dan < drugi.dan;
            return mjesec < drugi.mjesec;
        }
        return godina < drugi.godina;
    }
    bool JednakSa(const Datum &drugi) const {
        return dan == drugi.dan && mjesec == drugi.mjesec && godina == drugi.godina;
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

int main() {
    Datum d1, d2;
    d1.PostaviDatum(9, 5, 2024);
    d2.PostaviDatum(9, 5, 2024);

    d1.IspisiDatumSNazivom();
    d2.IspisiDatumSNazivom();
    std::cout << "\nDan u sedmici datuma ";
    d2.IspisiDatumSNazivom();
    std::cout << "je " << d2.DajDanUSedmici() << "\n\n";
    std::cout << "Jednaki d1 i d2 - " << std::boolalpha << JednakiDatumi(d1, d2) << "\n";
    d2.PomjeriDatumZa(5);
    std::cout << "Pomjeramo za 5 dana: ";
    d2.IspisiDatumSNazivom();
    std::cout << "Njihova razlika je: " << ProtekloDana(d1, d2) << "\n";
    return 0;
}
