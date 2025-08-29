#include <cctype>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <fstream>

struct Radnik {
  char ime_i_prezime[20], adresa[50];
  int telefon;
};

class Imenik {
  int kapacitet, broj_evidentiranih;
  Radnik *radnici;
public:
  explicit Imenik(int kapacitet) : broj_evidentiranih(0),
    kapacitet(kapacitet), radnici(new Radnik[kapacitet]) {}

  ~Imenik() { delete[] radnici; }

  Imenik(const Imenik &i) : broj_evidentiranih(i.broj_evidentiranih),
    kapacitet(i.kapacitet), radnici(new Radnik[i.kapacitet]) {
    std::copy(i.radnici, i.radnici + kapacitet, radnici);
  }
  Imenik(Imenik&& i) : broj_evidentiranih(i.broj_evidentiranih),
    kapacitet(i.kapacitet), radnici(i.radnici) {}

  Imenik& operator=(const Imenik& i) {
    Radnik* noviRadnici = new Radnik[i.kapacitet];
    std::copy(i.radnici, i.radnici + i.kapacitet, noviRadnici);

    delete[] radnici;
    radnici = noviRadnici;
    broj_evidentiranih = i.broj_evidentiranih;
    kapacitet = i.kapacitet;
    return *this;
  }
  Imenik& operator=(Imenik&& i) {
    delete[] radnici;

    broj_evidentiranih = i.broj_evidentiranih;
    kapacitet = i.kapacitet;
    radnici = i.radnici;
    return *this;
  }

  void DodajRadnika(const Radnik &r) {
    if(broj_evidentiranih >= kapacitet)
      throw std::range_error("Dostignut kapacitet imenika!");
    radnici[broj_evidentiranih++] = r;
  }
  void DodajRadnika(const char* ime_i_prezime, const char* adresa, int telefon) {
    if(std::strlen(ime_i_prezime) > 20 || std::strlen(adresa) > 50) throw;
    Radnik r;
    std::copy(ime_i_prezime, 
              ime_i_prezime + strlen(ime_i_prezime), 
              r.ime_i_prezime);
    std::copy(adresa, adresa + strlen(adresa), r.adresa);
    r.telefon = telefon;
    DodajRadnika(r);
  }
  void DodajRadnika(Radnik* r) {
    DodajRadnika(*r);
  }

  void IspisiRadnika(const char* ime_i_prezime) {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(strcmp(ime_i_prezime, radnici[i].ime_i_prezime) == 0)
        IspisiRadnika(i);
  }
  void IspisiSveNaSlovo(char s) {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(radnici[i].ime_i_prezime[0] == std::tolower(s) 
        || radnici[i].ime_i_prezime[0] == std::toupper(s))
        IspisiRadnika(i);
  }
  void IspisiSortirano() {
    std::sort(radnici, radnici + broj_evidentiranih, 
      [](const Radnik& r1, const Radnik& r2) {
      return strcmp(r1.ime_i_prezime, r2.ime_i_prezime) < 0;
    });
    IspisiImenik();
    // Radnik* sortiraniRadnici = new Radnik[kapacitet];
    // std::copy(radnici, radnici + broj_evidentiranih, sortiraniRadnici);
    //
    // delete[] sortiraniRadnici;
  }
  int& operator[](const char* ime_i_prezime) {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(strcmp(ime_i_prezime, radnici[i].ime_i_prezime) == 0)
        return radnici[i].telefon;
    throw;
  }
  int operator[](const char* ime_i_prezime) const {
    for(int i = 0; i < broj_evidentiranih; i++)
      if(strcmp(ime_i_prezime, radnici[i].ime_i_prezime) == 0)
        return radnici[i].telefon;
    throw;
  }
  void IspisiImenik() const {
    for(int i = 0; i < broj_evidentiranih; i++)
      IspisiRadnika(i);
  }
  void IspisiRadnika(int index) const {
    std::cout << "Ime i prezime: " << radnici[index].ime_i_prezime << std::endl
      << "Adresa: " << radnici[index].adresa << std::endl
      << "Telefon: " << radnici[index].telefon << std::endl;
  }

  void Sacuvaj(const char* ime_datoteke) const {
    std::ofstream binarna(ime_datoteke, std::ios::out | std::ios::binary);

    for(int i = 0; i < broj_evidentiranih; i++)
      binarna.write(reinterpret_cast<char*>(&radnici[i]), sizeof radnici[i]);

    binarna.close();
  }

  void Obnovi(const char* ime_datoteke) {
    std::ifstream binarna(ime_datoteke, std::ios::in | std::ios::binary);

    Radnik radnik;
    broj_evidentiranih = 0;
    while(binarna.read(reinterpret_cast<char*>(&radnik), sizeof radnik))
      radnici[broj_evidentiranih++] = radnik;

    binarna.close();
  }
};
int main() {
  // Radnik r1{"Meho Mehic", "Zmaja od Bosne 13, Sarajevo", 1234};
  // Radnik* r2 = new Radnik{"Pero Peric", "Travnicka 15, Zenica", 4321};
  // Imenik i(10);
  // i.DodajRadnika(r1);
  // i.DodajRadnika(r2);
  // i.DodajRadnika("Bakir", "Celjigovici", 47);
  // i.IspisiSortirano();
  // i.Sacuvaj("niggafile.dat");
  // delete r2;
  Imenik i(10);
  i.Obnovi("niggafile.dat");
  i.IspisiImenik();
  return 0;
}
