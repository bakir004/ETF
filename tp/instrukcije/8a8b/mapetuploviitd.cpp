#include<iostream>
#include<map>
#include<unordered_map>

void NajveciBrojPojavljivanja(const std::vector<int> &niz) {
  std::unordered_map<int,int> brojPojavljivanja;
  for(int i = 0; i < niz.size(); i++) {
    brojPojavljivanja[niz[i]]++;
  }

  int brojKojiSeNajvisePojavljuje, najveciBrojPojavljivanja = 0;
  for(std::pair<int,int> par : brojPojavljivanja) {
    std::cout << par.first << " pojavljuje se " << par.second << " puta\n";
    int trenutniBroj = par.first;
    int brojPojavljivanjaTrenutnog = par.second;

    if(brojPojavljivanjaTrenutnog > najveciBrojPojavljivanja) {
      najveciBrojPojavljivanja = brojPojavljivanjaTrenutnog;
      brojKojiSeNajvisePojavljuje = trenutniBroj;
    }
  }

  std::cout << "Broj koji se najvise pojavljuje je " << brojKojiSeNajvisePojavljuje << " i to " << najveciBrojPojavljivanja << " puta\n";
}

int main() {
  std::vector<int> brojevi{1,2,3,3,3,3,999,999,999,4,5,6,6};
  NajveciBrojPojavljivanja(brojevi);

  // std::map<int, std::string> indeksiUcenika;
  // indeksiUcenika[19705] = "Bakir";
  // indeksiUcenika.insert({19529, "Alija"});
  //
  // std::cout << indeksiUcenika[19529];

  

  return 0;
}
