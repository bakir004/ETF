#include <cctype>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstring>

class Radnik {
  std::string ime_i_prezime, adresa;
  int telefon;
public:
  Radnik() {}
  Radnik(const char* ime_i_prezime, const char* adresa, int telefon)
    : ime_i_prezime(ime_i_prezime), adresa(adresa), telefon(telefon) {}
  std::string DajIme() const { return ime_i_prezime; }
  std::string DajAdresu() const { return adresa; }
  int& DajTelefon() { return telefon; }
};

class Imenik {
  int kapacitet, broj_evidentiranih;
  Radnik **radnici;
public:
  explicit Imenik(int kapacitet) : broj_evidentiranih(0),
    kapacitet(kapacitet), radnici(new Radnik*[kapacitet]{}) {
  std::cout << "Konstruktor\n";
  }

  ~Imenik() { 
    std::cout << "DESKT";
    for(int i = 0; i < kapacitet; i++) 
      delete radnici[i];
    delete[] radnici;
  }

  Imenik(const Imenik &i) : broj_evidentiranih(i.broj_evidentiranih),
    kapacitet(i.kapacitet), radnici(new Radnik*[i.kapacitet]{}) {
    try {
       for(int j = 0; j < kapacitet; j++)
         radnici[j] = new Radnik(*i.radnici[j]);
    } catch(std::exception& e){
      for(int i = 0; i < kapacitet; i++)
        delete radnici[i];
      delete[] radnici;
    }
  }
  Imenik(Imenik&& i) : broj_evidentiranih(i.broj_evidentiranih),
    kapacitet(i.kapacitet), radnici(i.radnici) {}

  Imenik& operator=(const Imenik& i) {
    Radnik** noviRadnici = new Radnik*[i.kapacitet]{};
    for(int j = 0; j < i.kapacitet; j++)
      noviRadnici[j] = new Radnik(*i.radnici[j]);

    for(int j = 0; j < kapacitet; j++)
      delete radnici[j];
    delete[] radnici;

    radnici = noviRadnici;
    broj_evidentiranih = i.broj_evidentiranih;
    kapacitet = i.kapacitet;
    return *this;
  }
  Imenik& operator=(Imenik&& i) {
    for(int i = 0; i < kapacitet; i++)
      delete radnici[i];
    delete[] radnici;

    broj_evidentiranih = i.broj_evidentiranih;
    kapacitet = i.kapacitet;
    radnici = i.radnici;
    return *this;
  }

  void DodajRadnika(const Radnik &r) {
    if(broj_evidentiranih >= kapacitet)
      throw std::range_error("Dostignut kapacitet imenika!");
    radnici[broj_evidentiranih++] = new Radnik(r);
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
    for(int i = 0; i < broj_evidentiranih; i++)
      if(std::string(ime_i_prezime) == radnici[i]->DajIme())
        IspisiRadnika(i);
  }
  void IspisiSveNaSlovo(char s) {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(radnici[i]->DajIme()[0] == std::tolower(s) 
        || radnici[i]->DajIme()[0] == std::toupper(s))
        IspisiRadnika(i);
  }
  void IspisiSortirano() {
    std::sort(radnici, radnici + broj_evidentiranih, 
      [](Radnik* r1, Radnik* r2) {
      return r1->DajIme() < r2->DajIme();
    });
    IspisiImenik();
    // Radnik* sortiraniRadnici = new Radnik[kapacitet];
    // std::copy(radnici, radnici + broj_evidentiranih, sortiraniRadnici);
    //
    // delete[] sortiraniRadnici;
  }
  int& operator[](const char* ime_i_prezime) {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(std::string(ime_i_prezime) == radnici[i]->DajIme())
        return radnici[i]->DajTelefon();
    throw;
  }
  int operator[](const char* ime_i_prezime) const {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(std::string(ime_i_prezime) == radnici[i]->DajIme())
        return radnici[i]->DajTelefon();
    throw;
  }
  void IspisiImenik() const {
    for(int i = 0; i < broj_evidentiranih; i++)
      IspisiRadnika(i);
  }
  void IspisiRadnika(int index) const {
    std::cout << "Ime i prezime: " << radnici[index]->DajIme()<< std::endl
      << "Adresa: " << radnici[index]->DajAdresu() << std::endl
      << "Telefon: " << radnici[index]->DajTelefon() << std::endl;
  }

  // void Sacuvaj(const char* ime_datoteke) const {
  //   std::ofstream binarna(ime_datoteke, std::ios::binary);
  //
  //   for(int i = 0; i < broj_evidentiranih; i++)
  //     binarna.write(reinterpret_cast<char*>(&*radnici[i]), sizeof *radnici[i]);
  //
  //   binarna.close();
  // }
  //
  // void Obnovi(const char* ime_datoteke) {
  //   std::cout << "Nigga";
  //   std::ifstream binarna(ime_datoteke, std::ios::binary);
  //
  //   Radnik radnik;
  //   broj_evidentiranih = 0;
  //   while(binarna.read(reinterpret_cast<char*>(&radnik), sizeof radnik)) {
  //     radnici[broj_evidentiranih++] = new Radnik(radnik);
  //     std::cout << "Ucitan " << radnik.DajIme();
  //   }
  //
  //   binarna.close();
  // }
};
int main() {
  // Radnik r1{"Meho Mehic", "Zmaja od Bosne 13, Sarajevo", 1234};
  // Radnik* r2 = new Radnik{"Pero Peric", "Travnicka 15, Zenica", 4321};
  // Imenik i(10);
  // i.DodajRadnika(r1);
  // i.DodajRadnika(r2);
  // i.DodajRadnika("Nigga", "Celjigovici", 47);
  // i.IspisiSortirano();
  // i.Sacuvaj("niggafile.dat");
  // delete r2;
  // Imenik i(10);
  // std::cout << "ASDK?";
  // i.Obnovi("niggafile.dat");
  // i.IspisiImenik();
  return 0;
}
