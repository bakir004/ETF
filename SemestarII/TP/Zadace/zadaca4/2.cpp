#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <string>

enum Pol {Musko, Zensko};
class GradjaninBiH {
    std::string ime;
    long long int jmbg;
    GradjaninBiH* prosli;
    static GradjaninBiH* kraj;
    bool ValidanJMBG(long long int jmbg);
    bool ValidanDatum(int dan, int mjesec, int godina);
public:
    static void IspisiLanac();
    GradjaninBiH(std::string ime, long long int jmbg);
    GradjaninBiH(std::string ime, int dan, int mjesec, int godina, int sifra, Pol pol);
    GradjaninBiH(const GradjaninBiH& d) = delete;
    GradjaninBiH& operator=(const GradjaninBiH& d) = delete;
    std::string DajImeIPrezime() const;
    long long int DajJMBG() const;
    int DajDanRodjenja() const;
    GradjaninBiH* DajPrethodnog() const;
    void PostaviPrethodnog(GradjaninBiH* p);
    int DajMjesecRodjenja() const;
    int DajGodinuRodjenja() const;
    int DajSifruRegije() const;
    int DajPol() const;
    int DajKod() const;
    void PromijeniImeIPrezime(std::string ime) { 
        if(ime.length() == 0) throw std::logic_error("Ime ne moze biti prazno!");
        this->ime = ime; 
    }
    ~GradjaninBiH();
};
GradjaninBiH* GradjaninBiH::kraj = nullptr;

void GradjaninBiH::PostaviPrethodnog(GradjaninBiH* p) {
    prosli = p;
}
GradjaninBiH* GradjaninBiH::DajPrethodnog() const {
    return prosli;
}
void GradjaninBiH::IspisiLanac() {
    auto pok = kraj;
    while(pok != nullptr) {
        std::cout << pok->DajImeIPrezime() << ":" << pok->DajKod() << " -> ";
        pok = pok->prosli;
    }
    std::cout << '\n';
}
GradjaninBiH::~GradjaninBiH() {
    // if(prosli == nullptr) {
    //     auto pok = kraj;
    //     while(pok->prosli != nullptr && pok->prosli->DajJMBG() != this->DajJMBG())
    //         pok = pok->prosli;
    //     pok->prosli = nullptr;
    // } else if(kraj->DajJMBG() == this->DajJMBG()) {
    //     kraj = prosli;
    // } else {
    //     auto pok = kraj;
    //     while(pok->prosli->DajJMBG() != this->DajJMBG())
    //         pok = pok->prosli;
    //     pok->prosli = prosli;
    // }
}

GradjaninBiH::GradjaninBiH(std::string ime, long long int jmbg) {
    if(ime.length() == 0) throw std::logic_error("Ime ne moze biti prazno!");
    if(!ValidanJMBG(jmbg)) throw std::logic_error("JMBG nije validan");

    if(kraj == nullptr) prosli = nullptr;
    else prosli = kraj;
    
    auto pok = this;
    while((pok=pok->prosli) != nullptr)
        if(pok->DajJMBG() == jmbg)
            throw std::logic_error("Vec postoji gradjanin sa istim JMBG");

    kraj = this;
    this->ime = ime;
    this->jmbg = jmbg;
    // std::cout << "Kreiran " << std::setfill(' ') <<  std::setw(20) << ime << " sa JMBG: " << std::setfill('0') << std::setw(13) << jmbg << "\n";
}

GradjaninBiH::GradjaninBiH(std::string ime, int dan, int mjesec, int godina, int sifra, Pol pol) {
    if(ime.length() == 0) throw std::logic_error("Ime ne moze biti prazno!");
    if(!ValidanDatum(dan, mjesec, godina) || sifra < 0 || sifra > 99)
        throw std::logic_error("Neispravni podaci");

    if(kraj == nullptr) prosli = nullptr;
    else prosli = kraj;

    bool zauzetiBrojevi[500];
    std::fill(zauzetiBrojevi, zauzetiBrojevi+500, 0);
    int offset = (pol == Pol::Zensko ? 500 : 0);

    auto pok = this;
    while((pok=pok->prosli) != nullptr)
        if(pok->DajPol() == pol && pok->DajDanRodjenja() == dan && pok->DajMjesecRodjenja() == mjesec && pok->DajGodinuRodjenja() == godina)
            zauzetiBrojevi[pok->DajKod()-offset] = true;
        
    
    int kod = 0;
    for(int i = 0; i < 500; i++) {
        if(!zauzetiBrojevi[i]) {
            kod = i;
            break;
        }
    }
    if(pol == Pol::Zensko) kod += 500;
    godina %= 1000;

    long long int jmbg = dan * 10e10 + mjesec * 10e8 + godina * 10e5 + sifra * 10e3 + kod * 10;
    int kontrolna = 11-(7*(dan/10+godina%10)+6*(dan%10+sifra/10)+5*(mjesec/10+sifra%10)+4*(mjesec%10+kod/100)+3*(godina/100+kod/10%10)+2*(godina/10%10+kod%10))%11;
    if(kontrolna == 11) kontrolna = 0;
    if(kontrolna == 10) {
        kod++;
        jmbg = dan * 10e10 + mjesec * 10e8 + godina * 10e5 + sifra * 10e3 + kod * 10;
        kontrolna = 11-(7*(dan/10+godina%10)+6*(dan%10+sifra/10)+5*(mjesec/10+sifra%10)+4*(mjesec%10+kod/100)+3*(godina/100+kod/10%10)+2*(godina/10%10+kod%10))%11;
    } 
    jmbg += kontrolna;
    
    kraj = this;
    this->ime = ime;
    this->jmbg = jmbg;
    // std::cout << "Kreiran " << std::setfill(' ') <<  std::setw(20) << ime << " sa JMBG: " << std::setfill('0') << std::setw(13) << jmbg << "\n";
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
    if(!ValidanDatum(dan, mjesec, godina)) return false;

    return true;
}
std::string GradjaninBiH::DajImeIPrezime() const { return ime; }
long long int GradjaninBiH::DajJMBG() const { return jmbg; }
int GradjaninBiH::DajSifruRegije() const { return jmbg / 10000 % 100; }
int GradjaninBiH::DajMjesecRodjenja() const { return int(jmbg / 10e8) % 100; }
int GradjaninBiH::DajDanRodjenja() const { return jmbg / 10e10; }
int GradjaninBiH::DajKod() const { return jmbg / 10 % 1000; }
int GradjaninBiH::DajGodinuRodjenja() const {
    int godina = int(jmbg / 10e5) % 1000;
    if(godina < 21) godina+=2000;
    else godina+=1000;
    return godina;
}
int GradjaninBiH::DajPol() const {
    if(DajKod() <= 499) return Pol::Musko;
    else return Pol::Zensko;
}

int main() {
    std::cout << "Unesite broj gradjana: \n";
    int n;
    std::cin >> n;
    GradjaninBiH **gradjani = new GradjaninBiH*[n]{};
    try {
        for(int i = 0; i < n; i++) {
            std::cout << "Unesite podatke za " << i+1 << ". gradjanina: \n";
            std::string ime;
            std::cout << "Ime i prezime: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, ime);
            int d,m,y;
            std::cout << "Datum rodjenja: ";
            std::cin >> d >> m >> y;
            int sifra;
            std::cout << "Sifra regije: ";
            std::cin >> sifra;
            int pol;
            std::cout << "Pol ( 1-Musko, 2-Zensko ): ";
            std::cin >> pol;
            try {
                gradjani[i] = new GradjaninBiH(ime, d, m, y, sifra, (pol == 1 ? Pol::Musko : Pol::Zensko));
            } catch(std::logic_error& err) {
                std::cout << err.what() << ". Molimo ponovite unos.\n";
                i--;
            }
        }
    } catch(...) {
        for(int i = 0; i < n; i++)
            delete gradjani[i];
        delete[] gradjani;
        return 0;
    }
    std::sort(gradjani, gradjani + n, [](GradjaninBiH* g1, GradjaninBiH* g2) {
            if(g1->DajGodinuRodjenja() == g2->DajGodinuRodjenja()) {
                if(g1->DajMjesecRodjenja() == g2->DajMjesecRodjenja())
                    return g1->DajDanRodjenja() < g2->DajDanRodjenja();
                return g1->DajMjesecRodjenja() < g2->DajMjesecRodjenja();
            }
            return g1->DajGodinuRodjenja() < g2->DajGodinuRodjenja();
            });
    std::cout << "Gradjani sortirani po starosti:\n";
    for(int i = 0; i < n; i++)
        std::cout << gradjani[i]->DajImeIPrezime() << " " << gradjani[i]->DajDanRodjenja() << "." << gradjani[i]->DajMjesecRodjenja() << "." << gradjani[i]->DajGodinuRodjenja() << " JMBG: " << std::setw(13) << std::setfill('0') << gradjani[i]->DajJMBG() << "\n";
    for(int i = 0; i < n; i++)
        delete gradjani[i];
    delete[] gradjani;
    return 0;
}
    
