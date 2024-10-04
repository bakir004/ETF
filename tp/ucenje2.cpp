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
int main() {
    StedniRacun* racun = new StedniRacun(20);
    SigurniRacun* s = dynamic_cast<SigurniRacun*>(racun);
    std::cout << !!s;
}
