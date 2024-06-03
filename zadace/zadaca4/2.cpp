#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

class GradjaninBiH {
    std::string ime;
    long long jmbg;
    GradjaninBiH* prosli;
    static GradjaninBiH* kraj;
    bool ValidanJMBG(long long int jmbg);
    bool ValidanDatum(int dan, int mjesec, int godina);
public:
    long long DajJMBG() const { return jmbg; }
    static void IspisiLanac();
    enum Pol {Musko, Zensko};
    GradjaninBiH(std::string ime, long long int jmbg);
    GradjaninBiH(std::string ime, int dan, int mjesec, int godina, int sifra, Pol pol);
    std::string DajImeIPrezime() { return ime; }
    long long DajJmbg() { return jmbg; }
    int DajDanRodjenja();
    int DajMjesecRodjenja();
    int DajGodinuRodjenja();
    int DajSifruRegije();
    int DajPol();
    int DajKod();
    void PromijeniImeIPrezime(std::string ime) { this->ime = ime; }
    ~GradjaninBiH();
};
GradjaninBiH* GradjaninBiH::kraj = nullptr;

void GradjaninBiH::IspisiLanac() {
    auto pok = kraj;
    while(pok != nullptr) {
        std::cout << pok->DajImeIPrezime() << ":" << pok->DajKod() << " -> ";
        pok = pok->prosli;
    }
    std::cout << '\n';
}
GradjaninBiH::~GradjaninBiH() {
    if(prosli == nullptr) {
        auto pok = kraj;
        while(pok->prosli->DajJmbg() != this->DajJmbg())
            pok = pok->prosli;
        pok->prosli = nullptr;
    } else if(kraj->DajJmbg() == this->DajJmbg()) {
        kraj = prosli;
    } else {
        auto pok = kraj;
        while(pok->prosli->DajJmbg() != this->DajJmbg())
            pok = pok->prosli;
        pok->prosli = prosli;
    }
}

GradjaninBiH::GradjaninBiH(std::string ime, long long int jmbg) {
    if(!ValidanJMBG(jmbg)) throw std::logic_error("JMBG nije validan");

    if(kraj == nullptr) prosli = nullptr;
    else prosli = kraj;
    
    auto pok = this;
    while((pok=pok->prosli) != nullptr)
        if(pok->DajJmbg() == jmbg)
            throw std::logic_error("Vec postoji gradjanin sa istim JMBG");

    kraj = this;
    this->ime = ime;
    this->jmbg = jmbg;
    std::cout << "Kreiran " << std::setfill(' ') <<  std::setw(20) << ime << " sa JMBG: " << std::setfill('0') << std::setw(13) << jmbg << "\n";
}

GradjaninBiH::GradjaninBiH(std::string ime, int dan, int mjesec, int godina, int sifra, Pol pol) {
    if(!ValidanDatum(dan, mjesec, godina) || sifra < 0 || sifra > 99)
        throw std::logic_error("Neispravni podaci");
    godina %= 1000;

    if(kraj == nullptr) prosli = nullptr;
    else prosli = kraj;

    bool zauzetiBrojevi[500];
    std::fill(zauzetiBrojevi, zauzetiBrojevi+500, 0);
    int offset = (pol == Pol::Zensko ? 500 : 0);

    auto pok = this;
    while((pok=pok->prosli) != nullptr)
        if(pok->DajPol() == pol)
            zauzetiBrojevi[pok->DajKod()-offset] = true;
    int kod = 0;
    for(int i = 0; i < 500; i++) {
        if(!zauzetiBrojevi[i]) {
            kod = i;
            break;
        }
    }
    if(pol == Pol::Zensko) kod += 500;

    long long int jmbg = dan * 10e10 + mjesec * 10e8 + godina * 10e5 + sifra * 10e3 + kod * 10;
    jmbg += 11-(7*(dan/10+godina%10)+6*(dan%10+sifra/10)+5*(mjesec/10+sifra%10)+4*(mjesec%10+kod/100)+3*(godina/100+kod/10%10)+2*(godina/10%10+kod%10))%11;

    kraj = this;
    this->ime = ime;
    this->jmbg = jmbg;
    std::cout << "Kreiran " << std::setfill(' ') <<  std::setw(20) << ime << " sa JMBG: " << std::setfill('0') << std::setw(13) << jmbg << "\n";
}

bool GradjaninBiH::ValidanDatum(int dan, int mjesec, int godina) {
    int brojDana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0)
        brojDana[1]++;
    if(godina < 1 || godina > 2021 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > brojDana[mjesec - 1])
        return false;
    return true;
}

bool GradjaninBiH::ValidanJMBG(long long int jmbg) {
    int dan = jmbg / 10e10;
    int mjesec = int(jmbg / 10e8) % 100;
    int godina = int(jmbg / 10e5) % 1000;
    int sifra = jmbg / 10000 % 100;
    int kod = jmbg / 10 % 1000;
    if(godina < 21) godina+=2000;
    else godina+=1000;
    int c13 = 11-(7*(dan/10+godina%10)+6*(dan%10+sifra/10)+5*(mjesec/10+sifra%10)+4*(mjesec%10+kod/100)+3*(godina/100+kod/10%10)+2*(godina/10%10+kod%10))%11;
    if(c13 == 11) c13 = 0;
    if(!ValidanDatum(dan, mjesec, godina) || c13 == 10) return false;

    return true;
}
int GradjaninBiH::DajSifruRegije() { return jmbg / 10000 % 100; }
int GradjaninBiH::DajMjesecRodjenja() { return int(jmbg / 10e8) % 100; }
int GradjaninBiH::DajDanRodjenja() { return jmbg / 10e10; }
int GradjaninBiH::DajKod() { return jmbg / 10 % 1000; }
int GradjaninBiH::DajGodinuRodjenja() {
    int godina = int(jmbg / 10e5) % 1000;
    if(godina < 21) godina+=2000;
    else godina+=1000;
    return godina;
}
int GradjaninBiH::DajPol() {
    if(DajKod() <= 499) return GradjaninBiH::Pol::Musko;
    else return GradjaninBiH::Pol::Zensko;
}

int main() {
    {
        GradjaninBiH g3("Zakir Cinjarevic", 104996170021);
        GradjaninBiH* g1 = new GradjaninBiH("Zakir Cinjarevic", 104995170009);
        GradjaninBiH* g2 = new GradjaninBiH("Zakir Cinjarevic", 104998170010);
        GradjaninBiH* g4 = new GradjaninBiH("Bakeroni Cin", 27,2,2004,17,GradjaninBiH::Pol::Musko);
        GradjaninBiH::IspisiLanac();
    }
    GradjaninBiH::IspisiLanac();
    // GradjaninBiH g4("Zakir Cinjarevic", 104995170031);
    // GradjaninBiH g5("Zakir Cinjarevic", 104995170042);
    // GradjaninBiH g6("Zakir Cinjarevic", 104995170058);
    // GradjaninBiH g7("Zakir Cinjarevic", 104995170068);
    // GradjaninBiH g2("Bakir Cinjarevic",27,2,2004,17,GradjaninBiH::Pol::Musko);
    // GradjaninBiH g8("Bakir Cinjarevic",27,2,2004,17,GradjaninBiH::Pol::Musko);
    // std::cout << g1.DajDanRodjenja() << " " << g1.DajMjesecRodjenja() << " " << g1.DajGodinuRodjenja() << " " << g1.DajSifruRegije() << " " << g1.DajKod() << "\n";
    // GradjaninBiH g3("Azra Cinjarevic",15,9,1992,17,GradjaninBiH::Pol::Zensko);
    // GradjaninBiH g4("Senad Cinjarevic", 12, 11, 1967, 17, GradjaninBiH::Pol::Musko);
    // GradjaninBiH g5("Zakira Cinjarevic", 21, 11, 1967, 17, GradjaninBiH::Pol::Zensko);
    // GradjaninBiH g6("Mirza Cinjarevic", 12, 11, 1967, 17, GradjaninBiH::Pol::Musko);
    // GradjaninBiH g7("Husein Cinjarevic", 12, 11, 1967, 17, GradjaninBiH::Pol::Musko);
    return 0;
}
