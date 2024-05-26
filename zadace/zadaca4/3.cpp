#include <iostream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

class Datum {
    int dan, mjesec, godina;
    void TestDatuma(int dan, int mjesec, int godina) const {
        int brojDana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) brojDana[1]++;
        if(godina < 1 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > brojDana[mjesec - 1])
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
    bool Prestupna() const { return godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0; }
    std::tuple<int,int,int> Ocitaj() const { return std::make_tuple(dan,mjesec,godina); };
    void Ispisi() const { std::cout << dan << "/" << mjesec << "/" << godina; }
};

class Vrijeme {
    int sati, minute;
    void TestVremena(int sati, int minute) const {
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
    std::pair<int,int> Ocitaj() const { return std::make_pair(sati, minute); }
    void Ispisi() const { std::cout << std::setw(2) << std::setfill('0') << sati << ":" << std::setw(2) << std::setfill('0') << minute; }
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
    void PomjeriDanUnaprijed();
    void PomjeriDanUnazad();
    std::string DajImePacijenta() const { return ime; }
    Datum DajDatumPregleda() const { return datum; }
    Vrijeme DajVrijemePregleda() const { return vrijeme; }
    static bool DolaziPrije(Pregled p1, Pregled p2);
    void Ispisi();
};

bool Pregled::DolaziPrije(Pregled p1, Pregled p2) {
    std::tuple<int,int,int> datumRazlozen1 = p1.datum.Ocitaj();
    int dan1 = std::get<0>(datumRazlozen1);
    int mjesec1 = std::get<1>(datumRazlozen1);
    int godina1 = std::get<2>(datumRazlozen1);
    std::pair<int,int> vrijemeRazlozeno1 = p1.vrijeme.Ocitaj();
    int minute1 = vrijemeRazlozeno1.second;
    int sati1 = vrijemeRazlozeno1.first;

    std::tuple<int,int,int> datumRazlozen2 = p2.datum.Ocitaj();
    int dan2 = std::get<0>(datumRazlozen2);
    int mjesec2 = std::get<1>(datumRazlozen2);
    int godina2 = std::get<2>(datumRazlozen2);
    std::pair<int,int> vrijemeRazlozeno2 = p2.vrijeme.Ocitaj();
    int minute2 = vrijemeRazlozeno2.second;
    int sati2 = vrijemeRazlozeno2.first;

    if(godina1 != godina2) return godina1 < godina2;
    if(mjesec1 != mjesec2) return mjesec1 < mjesec2;
    if(dan1 != dan2) return dan1 < dan2;
    if(sati1 != sati2) return sati1 < sati2;
    return minute1 < minute2;
}

void Pregled::Ispisi()  { 
    std::cout << std::left << std::setw(30) << std::setfill(' ') << ime;
    datum.Ispisi(); 
    std::cout << " "; 
    vrijeme.Ispisi(); 
    std::cout << "\n";
}

void Pregled::PomjeriDanUnazad() {
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
void Pregled::PomjeriDanUnaprijed()  {
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

class Pregledi {
    int brojPregleda;
    Pregled **pregledi;
    void ObrisiMe();
    void ProvjeraProstora() const;
    void ProvjeraImaLiPregleda() const;
public:
    explicit Pregledi(int max): brojPregleda(max), pregledi(new Pregled*[max]{}) {}
    Pregledi(std::initializer_list<Pregled> lista);
    Pregledi(const Pregledi& p);
    Pregledi(Pregledi&& p);
    Pregledi &operator=(const Pregledi& p);
    Pregledi &operator=(Pregledi&& p);
    ~Pregledi();
    void RegistrirajPregled(std::string ime, Datum d, Vrijeme v);
    void RegistrirajPregled(std::string ime, int dan, int mjesec, int godina, int sati, int minute);
    void RegistrirajPregled(Pregled *p);
    int DajBrojPregleda() const;
    int DajBrojPregledaNaDatum(Datum d) const;
    Pregled DajNajranijiPregled() const;
    Pregled &DajNajranijiPregled();
    void IsprazniKolekciju();
    void ObrisiNajranijiPregled();
    void ObrisiPregledePacijenta(std::string);
    void IspisiPregledeNaDatum(Datum d) const;
    void IspisiSvePreglede() const;
};

void Pregledi::IspisiSvePreglede() const {
    std::vector<Pregled*> vektorPregleda;
    for(int i = 0; i < brojPregleda; i++) 
        if(pregledi[i] != nullptr)
            vektorPregleda.push_back(pregledi[i]);
    sort(vektorPregleda.begin(), vektorPregleda.end(), [](Pregled* p1, Pregled* p2){
            return Pregled::DolaziPrije(*p1, *p2);
            });
    for(int i = 0; i < brojPregleda; i++)
        if(pregledi[i] != nullptr)
            (*pregledi[i]).Ispisi();
    std::cout << '\n';
}
void Pregledi::IspisiPregledeNaDatum(Datum d) const {
    std::vector<Pregled*> vektorPregleda;
    std::for_each(pregledi, pregledi + brojPregleda, [d, &vektorPregleda](Pregled* p){
            if(p == nullptr) return;
            std::tuple<int,int,int> ocitanDatum = p->DajDatumPregleda().Ocitaj();
            std::tuple<int,int,int> ocitanD = d.Ocitaj();
            if(ocitanD == ocitanDatum)
                vektorPregleda.push_back(p); 
            });
    sort(vektorPregleda.begin(), vektorPregleda.end(), [](Pregled* p1, Pregled* p2){
            return Pregled::DolaziPrije(*p1, *p2);
            });
    std::cout << "Na datum ";
    d.Ispisi();
    std::cout << ":\n";
    for(int i = 0; i < vektorPregleda.size(); i++)
        (*vektorPregleda[i]).Ispisi();
    std::cout << '\n';
}
void Pregledi::ObrisiPregledePacijenta(std::string ime) {
    std::for_each(pregledi, pregledi + brojPregleda, [ime](Pregled *p){
            if(p->DajImePacijenta() == ime) {
                delete p;
                p = nullptr;
            }
            });
}
void Pregledi::ObrisiNajranijiPregled() {
    int broj = std::count(pregledi, pregledi+brojPregleda, nullptr);   
    if(broj == brojPregleda) throw std::range_error("Prazna kolekcija");

    Pregled **najraniji = std::min_element(pregledi, pregledi + brojPregleda, [](Pregled *p1, Pregled *p2){
            if(p1 == nullptr) return false;
            if(p2 == nullptr) return true;
            return Pregled::DolaziPrije(*p1, *p2);
            });
    delete *najraniji;
    *najraniji = nullptr;
}

void Pregledi::IsprazniKolekciju() {
    for(int i = 0; i < brojPregleda; i++) {
        delete pregledi[i];
        pregledi[i] = nullptr;
    }
}
void Pregledi::ProvjeraImaLiPregleda() const {
    int broj = std::count(pregledi, pregledi+brojPregleda, nullptr);   
    if(broj == brojPregleda) throw std::domain_error("Nema registriranih pregleda");
}

Pregled Pregledi::DajNajranijiPregled() const {
    Pregled najraniji = **std::min_element(pregledi, pregledi + brojPregleda, [](Pregled *p1, Pregled *p2){
            if(p1 == nullptr) return false;
            if(p2 == nullptr) return true;
            return Pregled::DolaziPrije(*p1, *p2);
            });
    return najraniji;
}
Pregled& Pregledi::DajNajranijiPregled() {
    Pregled& najraniji = **std::min_element(pregledi, pregledi + brojPregleda, [](Pregled *p1, Pregled *p2){
            if(p1 == nullptr) return false;
            if(p2 == nullptr) return true;
            return Pregled::DolaziPrije(*p1, *p2);
            });
    return najraniji;
}

int Pregledi::DajBrojPregleda() const {
    return brojPregleda - std::count(pregledi, pregledi+brojPregleda, nullptr);
}
int Pregledi::DajBrojPregledaNaDatum(Datum d) const {
    return std::count_if(pregledi, pregledi+brojPregleda, [d](Pregled *p){
            std::tuple<int,int,int> razlozenDatum = p->DajDatumPregleda().Ocitaj();
            std::tuple<int,int,int> razlozenD = d.Ocitaj();
            return razlozenDatum == razlozenD;
            });
}

void Pregledi::ProvjeraProstora() const {
    int brojNealociranih = std::count(pregledi, pregledi+brojPregleda, nullptr);   
    if(brojNealociranih == 0) throw std::range_error("Dostignut maksimalni broj pregleda");
}

void Pregledi::RegistrirajPregled(Pregled *p) {
    ProvjeraProstora();
    for(int i = 0; i < brojPregleda; i++) {
        if(pregledi[i] == nullptr) {
            // p mora biti dinamicki alociran
            pregledi[i] = p;
            break;
        }
    }
}

void Pregledi::RegistrirajPregled(std::string ime, int dan, int mjesec, int godina, int sati, int minute) {
    ProvjeraProstora();
    for(int i = 0; i < brojPregleda; i++) {
        if(pregledi[i] == nullptr) {
            pregledi[i] = new Pregled(ime,dan,mjesec,godina,sati,minute);
            break;
        }
    }
}
void Pregledi::RegistrirajPregled(std::string ime, Datum d, Vrijeme v) {
    ProvjeraProstora();
    for(int i = 0; i < brojPregleda; i++) {
        if(pregledi[i] == nullptr) {
            pregledi[i] = new Pregled(ime,d,v);
            break;
        }
    }
}

void Pregledi::ObrisiMe() {
    for(int i = 0; i < brojPregleda; i++)
        delete pregledi[i];
    delete[] pregledi;
    brojPregleda = 0;
    std::cout << "Obriso!\n";
}

Pregledi::Pregledi(const Pregledi& p): brojPregleda(p.brojPregleda), pregledi(new Pregled*[brojPregleda]{}) {
    try {
        for(int i = 0; i < brojPregleda; i++)
            pregledi[i] = new Pregled(*(p.pregledi[i]));
    } catch(...) {
        ObrisiMe();
        throw;
    }
}

Pregledi& Pregledi::operator=(const Pregledi& p) {
    brojPregleda = p.brojPregleda;
    ObrisiMe();
    try {
        for(int i = 0; i < brojPregleda; i++)
            pregledi[i] = new Pregled(*(p.pregledi[i]));
    } catch(...) {
        ObrisiMe();
        throw;
    }
    return *this;
}
Pregledi& Pregledi::operator=(Pregledi&& p) {
    ObrisiMe();
    brojPregleda = p.brojPregleda;
    pregledi = p.pregledi;
    p.pregledi = nullptr;
    return *this;
}

Pregledi::Pregledi(Pregledi&& p): brojPregleda(p.brojPregleda), pregledi(p.pregledi) { p.pregledi = nullptr; }

Pregledi::Pregledi(std::initializer_list<Pregled> lista): brojPregleda(lista.size()), pregledi(new Pregled*[brojPregleda]{}) {
    try {
        int i = 0;
        for(auto it = lista.begin(); it != lista.end(); i++, it++)
            pregledi[i] = new Pregled(*it);
    } catch(...) {
        ObrisiMe();
        throw;
    }
}

Pregledi::~Pregledi() { ObrisiMe(); }

int main() {
    Pregledi pregledi(5);
    pregledi.RegistrirajPregled("Bakir Cinjarevic", 26,5,2024,11,30);
    pregledi.RegistrirajPregled("Zakir Cinjarevic", 26,5,2024,12,30);
    pregledi.RegistrirajPregled("Azra Cinjarevic", 26,5,2024,10,30);
    pregledi.RegistrirajPregled("Zakira Cinjarevic", 27,5,2024,11,30);
    pregledi.IspisiPregledeNaDatum({27,5,2024});
    pregledi.DajNajranijiPregled().Ispisi();
    return 0;
}
