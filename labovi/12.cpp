#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

class Datum {
    int dan, mjesec, godina;
    std::vector<std::string> naziviMjeseci{"Januar", "Februar", "Mart", "April", "Maj", "Juni", "Juli", "August", "Septembar", "Oktobar", "Novembar", "Decembar"};
    std::vector<std::string> naziviDana{"Ponedjeljak", "Utorak", "Srijeda", "Cetvrtak", "Petak", "Subota", "Nedjelja"};
    int DatumUDane() const {
        int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
        int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0)
            broj_dana[1]++;
        if(godina < 1 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > broj_dana[mjesec - 1])
            throw std::domain_error("Neispravan datum!");
        this->dan = dan; this->mjesec = mjesec; this->godina = godina;
    }
    void PostaviDan(int dan) {
        int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
        if(godina < 1)
            throw std::domain_error("Neispravan mjesec!");
        this->godina = godina;
    }
    Datum operator+(int n) {
        Datum novi = *this;
        novi.PomjeriDatumZa(n);
        return novi;
    }
    Datum operator-(int n) {
        Datum novi = *this;
        novi.PomjeriDatumZa(-n);
        return novi;
    }
    Datum& operator+=(int n) {
        PomjeriDatumZa(n);
        return *this;
    }
    Datum& operator-=(int n) {
        PomjeriDatumZa(-n);
        return *this;
    }
    Datum operator+(const Datum& drugi) {
        int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        Datum novi = *this;
        novi.dan += drugi.dan;
        novi.mjesec += drugi.mjesec;
        novi.godina += drugi.godina;
        if(novi.mjesec > 12) novi.godina++, novi.mjesec-=12;
        if(novi.dan > broj_dana[novi.mjesec-1]) novi.mjesec++, novi.dan-=broj_dana[novi.mjesec-1];
        return novi;
    }
    Datum& operator+=(const Datum& drugi) {
        int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        this->dan += drugi.dan;
        this->mjesec += drugi.mjesec;
        this->godina += drugi.godina;
        if(this->mjesec > 12) this->godina++, this->mjesec-=12;
        if(this->dan > broj_dana[this->mjesec-1]) this->mjesec++, this->dan-=broj_dana[this->mjesec-1];
        return *this;
    }
    int operator-(const Datum& drugi) {
        return this->DatumUDane() - drugi.DatumUDane();
    }
    Datum& operator-=(const Datum& drugi) {
        int razlika = this->DatumUDane() - drugi.DatumUDane();
        PomjeriDatumZa(-razlika);
        return *this;
    }
    bool operator>(const Datum& drugi) {
        if(godina == drugi.DajGodinu()) {
            if(mjesec == drugi.DajMjesec())
                return dan > drugi.DajDan();
            return mjesec > drugi.DajMjesec();
        }
        return godina > drugi.DajGodinu();
    }
    bool operator<(const Datum& drugi) {
        if(godina == drugi.DajGodinu()) {
            if(mjesec == drugi.DajMjesec())
                return dan < drugi.DajDan();
            return mjesec < drugi.DajMjesec();
        }
        return godina < drugi.DajGodinu();
    }
    bool operator==(const Datum& drugi) {
        return dan == drugi.DajDan() && mjesec == drugi.DajMjesec() && godina == drugi.DajGodinu();
    }
    std::string operator%(int) {
        int dani = DatumUDane();
        return naziviDana[(dani-1)%7];
    }
    Datum operator++(int) {
        Datum pomocni = *this;
        *this += 1;
        return pomocni;
    }
    Datum& operator++() {
        return *this += 1;
    }
    Datum operator--(int) {
        Datum pomocni = *this;
        *this -= 1;
        return pomocni;
    }
    Datum& operator--() {
        return *this -= 1;
    }
    int operator[](int x) const {
        if(x == 1) return dan;
        if(x == 2) return mjesec;
        if(x == 3) return godina;
        throw std::range_error("Izvan opsega");
    }
    operator std::string() const {
        return std::to_string(dan) + '.' + std::to_string(mjesec) + '.' + std::to_string(godina) + '.';
    }
    friend std::ostream& operator<<(std::ostream& tok, const Datum& d);
    friend std::istream& operator>>(std::istream& tok, Datum& d);

    void PomjeriDatumZa(int brojDana) {
        int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
                if(dan == 0) {
                    mjesec--;
                    if(mjesec == 0) {
                        godina--;
                        mjesec = 12;
                        dan = 31;
                    } else {
                        dan = broj_dana[mjesec-1];
                    }
                } 
            }
        }
    }
    bool PrestupnaGodina(int godina) const {
        return godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0;
    }
    int DajDan() const { return dan; }
    int DajMjesec() const { return mjesec; }
    int DajGodinu() const { return godina; }
    // std::string DajNazivMjeseca() const {
    //     return naziviMjeseci[mjesec-1];
    // }
    // void IspisiDatumSNazivom() const {
    //     std::cout << std::setw(2) << std::setfill('0') << dan << ". " << DajNazivMjeseca() << " " << std::setw(4) << std::setfill('0') << godina << ".\n";
    // }
    // friend bool VeciDatum(const Datum &d1, const Datum &d2);
    // friend bool JednakiDatumi(const Datum &d1, const Datum &d2);
    // friend int ProtekloDana(const Datum &d1, const Datum &d2);
};

std::istream& operator>>(std::istream& tok, Datum& d) {
    char c;
    std::cin >> d.dan;
    std::cin >> c;
    if(c != '.') std::cin.setstate(std::ios::failbit);
    std::cin >> d.mjesec;
    std::cin >> c;
    if(c != '.') std::cin.setstate(std::ios::failbit);
    std::cin >> d.godina;
    int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(d.godina % 4 == 0 && d.godina % 100 != 0 || d.godina % 400 == 0)
        broj_dana[1]++;
    if(d.godina < 1 || d.dan < 1 || d.mjesec < 1 || d.mjesec > 12 || d.dan > broj_dana[d.mjesec - 1])
        std::cin.setstate(std::ios::failbit);
    return tok;
}
std::ostream& operator<<(std::ostream& tok, const Datum& d) {
    std::cout << std::setw(2) << std::setfill('0') << d.dan << "." << std::setw(2) << std::setfill('0') << d.mjesec << "." << std::setw(4) << std::setfill('0') << d.godina;
    return tok;
}

int main() {
    Datum d1(27,2,2004), d2(1,1,2004);
    Datum d3 = d1 - 3;
    std::cout << "D1: " << d1 << "\n";
    std::cout << "D3: " << d3 << "\n";
    Datum d4 = d1 += d2;
    std::cout << "D4: " << d4 << "\n";
    d4++;
    std::cout << "D4: " << d4 << "\n";
    std::cout << "D4: " << --d4 << "\n";
    std::cout << "D4: " << std::string(d4) << "\n\n";

    std::cout << "D1: " << d1 << "\n";
    std::cout << "D2: " << d2 << "\n";
    if(d1 < d2) std::cout << "D1 < D2\n";
    else std::cout << "D2 < D1\n";
    Datum d5;
    std::cin >> d5;
    if(std::cin) std::cout << d5;
    return 0;
}


// class Praznici {
//     int ukupno, poMjesecima[12]{};
//     Datum **datumi;
// public:
//     explicit Praznici(int maks): ukupno(maks), datumi(new Datum*[ukupno]{}) {
//         std::fill(std::begin(poMjesecima), std::end(poMjesecima), 0);
//     };
//     Praznici(std::vector<Datum> datumi): ukupno(datumi.size()), datumi(new Datum*[ukupno]{}) {
//         try {
//             for(int i = 0; i < ukupno; i++) {
//                 this->datumi[i] = new Datum(datumi[i]);
//                 this->poMjesecima[datumi[i].DajMjesec()-1]++;
//             }
//         } catch(...) {
//             for(int i = 0; i < ukupno; i++)
//                 delete this->datumi[i];
//             delete[] this->datumi;
//             std::fill(std::begin(poMjesecima), std::end(poMjesecima), 0);
//             throw;
//         }
//     }
//     Praznici(const Praznici &p) {
//         ukupno = p.ukupno;
//         std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
//         Datum **noviDatumi = new Datum*[ukupno]{};
//         try {
//             for(int i = 0; i < ukupno; i++) {
//                 if(p.datumi[i] == nullptr)
//                     noviDatumi[i] = nullptr;
//                 else
//                     noviDatumi[i] = new Datum(*(p.datumi[i]));
//             }
//         } catch(...) {
//             for(int i = 0; i < ukupno; i++)
//                 delete noviDatumi[i];
//             delete[] noviDatumi;
//             throw;
//         }
//         datumi = noviDatumi;
//     }
//     Praznici(Praznici &&p) {
//         if(ukupno != p.ukupno) throw std::domain_error("Nesaglasne dimenzije");
//         std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
//         datumi = p.datumi;
//         p.datumi = nullptr;
//         p.ukupno = 0;
//     }
//     Praznici &operator =(const Praznici &p) {
//         if(ukupno != p.ukupno) throw std::domain_error("Nasaglasne dimenzije");
//         // brisanje postojeceg
//         for(int i = 0; i < ukupno; i++)
//             delete datumi[i];
//         delete[] datumi;
//
//         // duboko kopiranje iz novog
//         ukupno = p.ukupno;
//         std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
//         Datum **noviDatumi = new Datum*[ukupno]{};
//         try {
//             for(int i = 0; i < ukupno; i++)
//                 noviDatumi[i] = new Datum(*(p.datumi[i]));
//         } catch(...) {
//             for(int i = 0; i < ukupno; i++)
//                 delete noviDatumi[i];
//             delete[] noviDatumi;
//             throw;
//         }
//         datumi = noviDatumi;
//         return *this;
//     }
//     Praznici &operator =(Praznici &&p) {
//         if(ukupno != p.ukupno) throw std::domain_error("Nesaglasne dimenzije");
//         // brisanje postojeceg
//         for(int i = 0; i < ukupno; i++)
//             delete datumi[i];
//         delete[] datumi;
//
//         // kradja
//         ukupno = p.ukupno;
//         std::copy(std::begin(p.poMjesecima), std::end(p.poMjesecima), std::begin(poMjesecima));
//         datumi = p.datumi;
//         p.datumi = nullptr;
//         return *this;
//     }
//     void DodajPraznik(Datum d) {
//         if(Praznik(d)) throw std::logic_error("Postoji praznik");
//         int brojPraznika = 0;
//         for(int i = 0; i < 12; i++)
//             brojPraznika+=poMjesecima[i];
//         if(ukupno == brojPraznika) throw std::logic_error("Nema dovoljno mjesta");
//
//         for(int i = 0; i < ukupno; i++)
//             if(datumi[i] == nullptr) {
//                 datumi[i] = new Datum(d);
//                 break;
//             }
//         poMjesecima[d.DajMjesec()-1]++;
//     }
//     bool Praznik(Datum d) {
//         for(int i = 0; i < ukupno; i++)
//             if(datumi[i] != nullptr && JednakiDatumi(d, *(datumi[i])))
//                 return true;
//         return false;
//     }
//     void IspisiPraznike();
//     void IspisiPraznikeMjeseca(int m);
//     ~Praznici() {
//         for(int i = 0; i < ukupno; i++)
//             delete datumi[i];
//         delete[] datumi;
//     };
// };
//
// void Praznici::IspisiPraznike() {
//     std::vector<Datum*> vektorDatuma;
//     for(int i = 0; i < ukupno; i++)
//         if(datumi[i] != nullptr)
//             vektorDatuma.push_back(datumi[i]);
//     sort(vektorDatuma.begin(), vektorDatuma.end(), [](Datum* d1, Datum* d2){
//             return !VeciDatum(*d1, *d2);
//             });
//     for(int i = 0; i < vektorDatuma.size(); i++) {
//         vektorDatuma[i]->IspisiDatum();
//         std::cout << ", " << vektorDatuma[i]->DajDanUSedmici() << "\n";
//     }
//     std::cout << "\n";
// }
// void Praznici::IspisiPraznikeMjeseca(int m) {
//     std::vector<Datum*> vektorDatuma;
//     for(int i = 0; i < ukupno; i++)
//         if(datumi[i] != nullptr && datumi[i]->DajMjesec() == m)
//             vektorDatuma.push_back(datumi[i]);
//
//     sort(vektorDatuma.begin(), vektorDatuma.end(), [](Datum* d1, Datum* d2){
//             return !VeciDatum(*d1, *d2);
//             });
//     for(int i = 0; i < vektorDatuma.size(); i++) {
//         vektorDatuma[i]->IspisiDatum();
//         std::cout << ", " << vektorDatuma[i]->DajDanUSedmici() << "\n";
//     }
//     std::cout << "\n";
// }

