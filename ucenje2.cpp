#include <iostream>
class StedniRacun {
    int broj_racuna;
    double stanje;
    static int posljednji_broj_racuna;
    static void TestIznosa(double iznos) {
        Izuzetak(iznos < 0 || int(iznos * 100) != iznos * 100, "Neispravan iznos");
    }
    virtual std::ostream &PomocnaZaIspis(std::ostream &tok) const {
        return tok << "Broj racuna: " << broj_racuna << " Stanje: " << stanje << std::endl;
    }
    protected:
    static void Izuzetak(bool uvjet, const char tekst[]) {
        if(uvjet) throw std::logic_error(tekst);
    }
    public:
    explicit StedniRacun(int pocetno_stanje) {
        TestIznosa(pocetno_stanje);
        Izuzetak(pocetno_stanje < 10, "Nedovoljan iznos za kreiranje racuna");
        stanje = pocetno_stanje; broj_racuna = ++posljednji_broj_racuna;
    }
    virtual ~StedniRacun() {}
    StedniRacun(const StedniRacun &r) : stanje(r.stanje) {
        broj_racuna = ++posljednji_broj_racuna;
    }
    StedniRacun &operator =(const StedniRacun &r) { stanje = r.stanje; return *this; }
    int DajBrojRacuna() const { return broj_racuna; }
    virtual double operator *() const { return stanje; }
    virtual StedniRacun &operator +=(double iznos) {
        TestIznosa(iznos); stanje += iznos; return *this;
    }
    virtual StedniRacun &operator -=(double iznos) {
        TestIznosa(iznos); Izuzetak(stanje < iznos, "Nedovoljno stanje na racunu");
        stanje -= iznos; return *this;
    }
    virtual StedniRacun &operator *=(double kamatna_stopa) {
        Izuzetak(kamatna_stopa < 0, "Neispravna kamatna stopa");
        stanje += stanje * kamatna_stopa / 100; return *this;
    }
    StedniRacun operator +(double iznos) { return StedniRacun(*this) += iznos; }
    StedniRacun operator -(double iznos) { return StedniRacun(*this) -= iznos; }
    StedniRacun operator *(double iznos) { return StedniRacun(*this) *= iznos; }
    friend std::ostream &operator <<(std::ostream &tok, const StedniRacun &s) {
        return s.PomocnaZaIspis(tok);
    }
};
int StedniRacun::posljednji_broj_racuna = 1000;
class SigurniRacun : public StedniRacun {
    int PIN;
    mutable bool otvoren;
    void TestOtvoren() const { Izuzetak(!otvoren, "Racun nije otvoren"); }
    std::ostream &PomocnaZaIspis(std::ostream &tok) const override {
        TestOtvoren();
        return tok << "PIN: “ << PIN << Broj racuna: " << DajBrojRacuna()
            << " Stanje: " << SigurniRacun::operator *() << std::endl;
    }
    friend class Banka;
    public:
    SigurniRacun(int pocetno_stanje, int PIN) : StedniRacun(pocetno_stanje),
    PIN(PIN), otvoren(false) { Izuzetak(PIN < 0 || PIN > 9999, "Neispravan PIN"); }
    void OtvoriRacun(int PIN) const {
        Izuzetak(PIN != SigurniRacun::PIN, "Neispravan PIN");
        otvoren = true;
    }
    void ZatvoriRacun() const { otvoren = false; }
    double operator *() const override { TestOtvoren(); return StedniRacun::operator *(); }
    StedniRacun &operator +=(double iznos) override {
        TestOtvoren(); return StedniRacun::operator +=(iznos);
    }
    StedniRacun &operator -=(double iznos) override {
        TestOtvoren(); return StedniRacun::operator -=(iznos);
    }
    StedniRacun &operator *=(double kamatna_stopa) override {
        TestOtvoren(); return StedniRacun::operator *=(kamatna_stopa);
    }
};
class Banka {
    int ukupno_racuna, kapacitet;
    StedniRacun **racuni;
    int PIN_upravitelja;
    StedniRacun *trenutno_otvoreni;
    static void Izuzetak(bool uvjet, const char tekst[]) {
        if(uvjet) throw std::logic_error(tekst);
    }
    int NadjiRacun(int broj_racuna) const;
    void RealocirajPoPotrebi();
    void Dealociraj();
    StedniRacun &DajTrenutnoOtvoreni() const {
        Izuzetak(!trenutno_otvoreni, "Racun nije otvoren");
        return *trenutno_otvoreni;
    }
public:
    Banka(int PIN) : kapacitet(100), ukupno_racuna(0), PIN_upravitelja(PIN),
    trenutno_otvoreni(nullptr), racuni(new StedniRacun*[100]{}) {
        Izuzetak(PIN < 0 || PIN > 9999, "Neispravan PIN");
    }
    ~Banka() { Dealociraj(); }
    Banka(const Banka &b);
    Banka(Banka &&b) : kapacitet(b.kapacitet), ukupno_racuna(b. ukupno_racuna),
    racuni(b.racuni), PIN_upravitelja(b.PIN_upravitelja),
    trenutno_otvoreni(b.trenutno_otvoreni) { b.ukupno_racuna = 0; b.racuni = nullptr; }
    Banka &operator =(const Banka &b) = delete;
    static bool DaLiJeSiguran(StedniRacun *r) { return dynamic_cast<SigurniRacun *>(r); }
    void KreirajRacun(int pocetni_iznos) {
        RealocirajPoPotrebi(); racuni[ukupno_racuna++] = new StedniRacun(pocetni_iznos);
    }
    void KreirajRacun(int pocetni_iznos, int PIN) {
        RealocirajPoPotrebi(); racuni[ukupno_racuna++] = new SigurniRacun(pocetni_iznos, PIN);
    }
    void ObrisiRacun(int broj_racuna);
    void OtvoriRacun(int broj_racuna);
    void OtvoriRacun(int broj_racuna, int PIN);
    double operator *() const { return *DajTrenutnoOtvoreni(); }
    StedniRacun &operator +=(double iznos) { return DajTrenutnoOtvoreni() += iznos; }
    StedniRacun &operator -=(double iznos) { return DajTrenutnoOtvoreni() -= iznos; }
    void ZatvoriRacun();
    void ObracunajKamate(int PIN, double kamatna_stopa);
    StedniRacun &operator [](int broj_racuna) { return *racuni[NadjiRacun(broj_racuna)]; }
    const StedniRacun &operator [](int broj_racuna) const {
        return *racuni[NadjiRacun(broj_racuna)];
    }
    friend std::ostream operator <<(std::ostream &tok, const Banka &b);
    void SacuvajUDatoteku(std::string ime_datoteke);
    Banka(std::string ime_datoteke);
};
int Banka::NadjiRacun(int broj_racuna) const {
    StedniRacun *p =std::find(racuni, racuni + ukupno_racuna,
            [broj_racuna](StedniRacun *r) { return r->DajBrojRacuna() == broj_racuna; });
    Izuzetak(p == racuni + ukupno_racuna, "Racun nije nadjen");
    return p - broj_racuna;
}
void Banka::RealocirajPoPotrebi() {
    if(ukupno_racuna == kapacitet) {
        kapacitet += 100;
        StedniRacun **novi = new StedniRacun*[kapacitet]{};
        std::copy(racuni, racuni + ukupno_racuna, novi);
        delete[] racuni;
        racuni = novi;
    }
}
void Banka::Dealociraj() {
    for(int i = 0; i < koliko; i++) delete racuni[i];
    delete[] racuni;
}
Banka::Banka(const Banka &b) : kapacitet(b.kapacitet), ukupno_racuna(b.ukupno_racuna),
    PIN_upravitelja(b.PIN_upravitelja), trenutno_otvoreni(b.trenutno_otvoreni),
    racuni(new StedniRacun*[b.kapacitet]) {
        try {
            for(int i = 0; i < ukupno_racuna; i++)
                if(!DaLiJeSiguran(b.racuni[i]))
                    racuni[i] = new StedniRacun(*b.racuni[i]);
                else
                    racuni[i] = new SigurniRacun(*static_cast<SigurniRacun*>(b.racuni[i]));
        }
        catch(...) {
            Dealociraj(); throw;
        }
    }
void Banka::ObrisiRacun(int broj_racuna) {
    int gdje_je = NadjiRacun(broj_racuna);
    delete racuni[gdje_je];
    racuni[gdje_je] = racuni[ukupno_racuna];
    racuni[ukupno_racuna--] = nullptr;
}
void Banka::OtvoriRacun(int broj_racuna) {
    Izuzetak(trenutno_otvoreni != nullptr, "Vec je otvoren neki racun");
    StedniRacun *racun = racuni[NadjiRacun(broj_racuna)];
    Izuzetak(DaLiJeSiguran(racun), "Racun je zasticen, neophodan je PIN");
    trenutno_otvoreni = racun;
}
void Banka::OtvoriRacun(int broj_racuna, int PIN) {
    Izuzetak(trenutno_otvoreni != nullptr, "Vec je otvoren neki racun");
    StedniRacun *racun = racuni[NadjiRacun(broj_racuna)];
    if(DaLiJeSiguran(racun)) {
        SigurniRacun *sr = static_cast<SigurniRacun*>(racun);
        if(PIN == PIN_upravitelja) PIN = sr->PIN;
        sr->OtvoriRacun(PIN);
    }
    trenutno_otvoreni = racun;
}
void ZatvoriRacun() {
    if(trenutno_otvoreni != nullptr && DaLiJeSiguran(trenutno_otvoreni))
        static_cast<SigurniRacun*>(trenutno_otvoreni)->ZatvoriRacun();
    trenutno_otvoreni = nullptr;
}

void Banka::ObracunajKamate(int PIN, double kamatna_stopa) {
    Izuzetak(PIN != PIN_upravitelja, "Potrebne su privilegije upravitelja");
    Izuzetak(kamatna_stopa < 0, "Neispravna kamatna stopa");
    SigurniRacun *pomocna = trenutno_otvoreni;
    for(int i = 0; i < ukupno_racuna; i++) {
        OtvoriRacun(racuni[i]->DajBrojRacuna(), PIN);
        (*trenutno_otvoreni) *= kamatna_stopa;
        ZatvoriRacun();
    }
    trenutno_otvoreni = pomocna;
}
void Banka::ObracunajKamate(int PIN, double kamatna_stopa) {
    Izuzetak(PIN != PIN_upravitelja, "Potrebne su privilegije upravitelja");
    Izuzetak(kamatna_stopa < 0, "Neispravna kamatna stopa");
    for(int i = 0; i < ukupno_racuna; i++)
        if(DaLiJeSiguran(racuni[i])) {
            SigurniRacun *sr = static_cast<SigurniRacun*>(racuni[i]);
            sr->OtvoriRacun(sr->PIN);
            (*sr) *= kamatna_stopa;
            sr->ZatvoriRacun();
        }
        else (*racuni[i]) *= kamatna_stopa;
}
friend std::ostream operator <<(std::ostream &tok, const Banka &b) {
    SigurniRacun *pomocna = trenutno_otvoreni;
    for(int i = 0; i < ukupno_racuna; i++) {
        OtvoriRacun(racuni[i]->DajBrojRacuna(), PIN_upravitelja);
        tok << *trenutno_otvoreni;
        ZatvoriRacun();
    }
    trenutno_otvoreni = pomocna;
}
void Banka::SacuvajUDatoteku(std::string ime_datoteke) {
    std::ofstream tok(ime_datoteke);
    Izuzetak(!tok, "Problemi pri kreiranju datoteke");
    tok << PIN_upravitelja << std::endl;
    SigurniRacun *pomocna = trenutno_otvoreni;
    for(int i = 0; i < ukupno_racuna; i++) {
        OtvoriRacun(racuni[i]->DajBrojRacuna(), PIN_upravitelja);
        tok << *(*trenutno_otvoreni);
        if(DaLiJeSiguran(racuni[i]))
            tok << "," << static_cast<SigurniRacun*>(racuni[i])->PIN;
        tok << std::endl;
        ZatvoriRacun();
    }
    trenutno_otvoreni = pomocna;
    Izuzetak(!tok, "Problemi pri upisu u datoteku");
}
Banka::Banka(std::string ime_datoteke) {
    std::ifstream tok(ime_datoteke);
    Izuzetak(!tok, "Datoteka vjerovatno ne postoji");
    tok >> PIN_upravitelja;
    int broj1, broj2;
    while(tok >> broj1) {
        while(tok.peek() == ' ') tok.get();
        if(tok.peek() != ',') KreirajRacun(broj1);
        else {
            tok.get(); tok >> broj2;
            KreirajRacun(broj1, broj2);
        }
    }
    Izuzetak(!tok.eof(), "Problemi pri citanju datoteke");
}
