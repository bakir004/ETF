#include <cctype>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cstring>
#include <fstream>
#include <vector>

class Radnik {
  std::string ime_i_prezime, adresa;
  int telefon;
public:
  Radnik() {}
  Radnik(const char* ime_i_prezime, const char* adresa, int telefon)
    : ime_i_prezime(ime_i_prezime), adresa(adresa), telefon(telefon) {}
  Radnik(std::string ime_i_prezime, std::string adresa, int telefon)
    : ime_i_prezime(ime_i_prezime), adresa(adresa), telefon(telefon) {}
  std::string DajIme() const { return ime_i_prezime; }
  std::string DajAdresu() const { return adresa; }
  int& DajTelefon() { return telefon; }
};

typedef std::shared_ptr<Radnik> PRadnik;

class Imenik {
  std::vector<PRadnik> radnici;
public:
  // PAZITI, TREBAJU UPRAVLJACKI ELEMENTI!!!!!!!!!
  void DodajRadnika(const Radnik &r) {
    PRadnik p(new Radnik(r));
    radnici.push_back(p);
  }
  void DodajRadnika(const char* ime_i_prezime, const char* adresa, int telefon) {
    if(std::strlen(ime_i_prezime) > 20 || std::strlen(adresa) > 50) throw;
    Radnik r(ime_i_prezime, adresa, telefon);
    DodajRadnika(r);
  }
  void DodajRadnika(Radnik* r) {
    DodajRadnika(*r);
  }

  void IspisiRadnika(const char* ime_i_prezime) {
    for(int i = 0; i < radnici.size(); i++)
      if(std::string(ime_i_prezime) == radnici[i]->DajIme())
        IspisiRadnika(i);
  }
  void IspisiSveNaSlovo(char s) {
    for(int i = 0; i < radnici.size(); i++)
      if(radnici[i]->DajIme()[0] == std::tolower(s) 
        || radnici[i]->DajIme()[0] == std::toupper(s))
        IspisiRadnika(i);
  }
  void IspisiSortirano() {
    std::sort(radnici.begin(), radnici.end(), 
      [](PRadnik r1, PRadnik r2) {
      return r1->DajIme() < r2->DajIme();
    });
    IspisiImenik();
    // Radnik* sortiraniRadnici = new Radnik[kapacitet];
    // std::copy(radnici, radnici + broj_evidentiranih, sortiraniRadnici);
    //
    // delete[] sortiraniRadnici;
  }
  int& operator[](const char* ime_i_prezime) {
    for(int i = 0; i < radnici.size(); i++)
      if(std::string(ime_i_prezime) == radnici[i]->DajIme())
        return radnici[i]->DajTelefon();
    throw;
  }
  int operator[](const char* ime_i_prezime) const {
    for(int i = 0; i < radnici.size(); i++)
      if(std::string(ime_i_prezime) == radnici[i]->DajIme())
        return radnici[i]->DajTelefon();
    throw;
  }
  void IspisiImenik() const {
    for(int i = 0; i < radnici.size(); i++)
      IspisiRadnika(i);
  }
  void IspisiRadnika(int index) const {
    std::cout << "Ime i prezime: " << radnici[index]->DajIme()<< std::endl
      << "Adresa: " << radnici[index]->DajAdresu() << std::endl
      << "Telefon: " << radnici[index]->DajTelefon() << std::endl;
  }

  void Sacuvaj(const char* ime_datoteke) const {
    std::ofstream dat(ime_datoteke);

    for(int i = 0; i < radnici.size(); i++) {
      dat << radnici[i]->DajIme() << "\n";
      dat << radnici[i]->DajAdresu() << "\n";
      dat << radnici[i]->DajTelefon() << "\n";
    }

    dat.close();
  }

  void Obnovi(const char* ime_datoteke) {
    std::ifstream dat(ime_datoteke);
    std::string ime, adresa;
    int telefon;
    while(getline(dat, ime)) {
      getline(dat, adresa);
      dat >> telefon;
      dat.get();
      PRadnik r(new Radnik(ime, adresa, telefon));
      radnici.push_back(r);
    }
    dat.close();
  }
};
int main() {
  // Radnik r1{"Meho Mehic", "Zmaja od Bosne 13, Sarajevo", 1234};
  // Radnik* r2 = new Radnik{"Pero Peric", "Travnicka 15, Zenica", 4321};
  // Imenik i;
  // i.DodajRadnika(r1);
  // i.DodajRadnika(r2);
  // i.DodajRadnika("Nigga", "Celjigovici", 47);
  // i.IspisiSortirano();
  // i.Sacuvaj("niggafile.txt");
  // delete r2;
  Imenik i;
  i.Obnovi("niggafile.txt");
  i.IspisiImenik();
  return 0;
}
