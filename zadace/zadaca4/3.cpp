#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <utility>

class Datum {
    int dan, mjesec, godina;
    void TestDatuma(int dan, int mjesec, int godina) {
        int brojDana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) brojDana[1]++;
        if(godina < 1 || godina > 2021 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > brojDana[mjesec - 1])
            throw std::domain_error("Neispravan datum");
    }
public:
    Datum(int dan, int mjesec, int godina) { Postavi(dan, mjesec, godina); }
    void Postavi(int dan, int mjesec, int godina) {
        TestDatuma(dan, mjesec, godina);
        this->dan = dan;
        this->mjesec = mjesec;
        this->godina = godina;
    }
    bool Prestupna() { return godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0; }
    std::tuple<int,int,int> Ocitaj() { return std::make_tuple(dan,mjesec,godina); };
    void Ispisi() { std::cout << dan << "/" << mjesec << "/" << godina; }
};

class Vrijeme {
    int sati, minute;
    void TestVremena(int sati, int minute) {
        if(sati < 1 || sati > 23 || minute < 1 || minute > 60)
            throw std::domain_error("Neispravno vrijeme");
    }
public:
    Vrijeme(int sati, int minute) { Postavi(sati, minute); }
    void Postavi(int sati, int minute) {
        TestVremena(sati, minute);
        this->sati = sati;
        this->minute = minute;
    }
    std::pair<int,int> Ocitaj() { return std::make_pair(sati, minute); }
    void Ispisi() { std::cout << std::setw(2) << std::setfill('0') << sati << ":" << std::setw(2) << std::setfill('0') << minute; }
};

class Pregled {
    std::string ime;
    Datum datum;
    Vrijeme vrijeme;
public:
    Pregled(std::string ime, Datum d, Vrijeme v): ime(ime), datum(d), vrijeme(v) {} 
    Pregled(std::string ime, int dan, int mjesec, int godina, int sati, int minute): ime(ime), datum(dan, mjesec, godina), vrijeme(sati, minute) {}
    Pregled &PromijeniPacijenta(std::string ime) { this->ime = ime; return *this;}
    Pregled &PromijeniDatum(Datum datum) { this->datum = datum; return *this; }
    Pregled &PromijeniVrijeme(Vrijeme vrijeme) { this->vrijeme = vrijeme; return *this; }
    void PomjeriDanUnaprijed() {
        int brojDana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        brojDana[1] += datum.Prestupna();
        std::tuple<int,int,int> ocitanDatum = datum.Ocitaj();
        int dan = std::get<0>(ocitanDatum);
        int mjesec = std::get<1>(ocitanDatum);
        int godina = std::get<2>(ocitanDatum);
        dan++;
        if(dan > brojDana[mjesec - 1]) dan = 1, mjesec++;
        if(mjesec > 12) mjesec = 1, godina++;
        datum = Datum(dan, mjesec, godina);
    }
    void PomjeriDanUnazad() {
        int brojDana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        brojDana[1] += datum.Prestupna();
        std::tuple<int,int,int> ocitanDatum = datum.Ocitaj();
        int dan = std::get<0>(ocitanDatum);
        int mjesec = std::get<1>(ocitanDatum);
        int godina = std::get<2>(ocitanDatum);
        dan--;
        if(dan < 1) mjesec--;
        if(mjesec < 1) mjesec = 12, dan = 31, godina--;
        else dan = brojDana[mjesec-1];
        datum = Datum(dan, mjesec, godina);
    }
    void Ispisi() { 
        std::cout << std::left << std::setw(30) << std::setfill(' ') << ime;
        datum.Ispisi(); 
        std::cout << " "; 
        vrijeme.Ispisi(); 
        std::cout << "\n";
    }
};

int main() {
    Pregled p1("Bakir Cinjarevic",{29,2,2004},{11,30});
    p1.PomjeriDanUnaprijed();
    p1.Ispisi();
    p1.PomjeriDanUnazad();
    p1.Ispisi();
    Pregled p2("Bakir Cinjarevic",29,2,2004,11,30);
    p2.Ispisi();
    return 0;
}
