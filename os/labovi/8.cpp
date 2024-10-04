#include <algorithm>
#include <iostream>
#include <map>

struct Vrijeme {
    int sati, minute, sekunde;
};

struct Kretanje {
    double put;
    Vrijeme pocetak, kraj;
    Kretanje *sljedeci;
};

struct KretanjeBezVeze {
    double put;
    Vrijeme pocetak;
    Vrijeme kraj;
};

struct Kriterij {
    bool operator()(const KretanjeBezVeze &k1, const KretanjeBezVeze &k2) const {
        return true;
    }
};

double VrijemeUSekunde(Vrijeme vrijeme) {
    return vrijeme.sati*3600 + vrijeme.minute*60 + vrijeme.sekunde;
}

int a = 1;
bool UnesiKretanje(Kretanje *trenutni) {
    std::cout << "Unesite put (0 za kraj): ";
    std::cin >> trenutni->put;
    if(trenutni->put <= 0) return false;
    // std::cout << "Unesite pocetno vrijeme puta (hh mm ss): ";
    // std::cin >> trenutni->pocetak.sati >> trenutni->pocetak.minute >> trenutni->pocetak.sekunde;
    // std::cout << "Unesite krajnja vrijeme puta (hh mm ss): ";
    // std::cin >> trenutni->kraj.sati >> trenutni->kraj.minute >> trenutni->kraj.sekunde;
    trenutni->pocetak.sati = a; a++;
    trenutni->pocetak.minute = a; a++;
    trenutni->pocetak.sekunde = a; a++;
    trenutni->kraj.sati = a; a++;
    trenutni->kraj.minute = a; a++;
    trenutni->kraj.sekunde = a; a++;
    return true;
}

int main() {
    Kretanje *pocetak = new Kretanje;
    Kretanje *prosli = pocetak;
    if(!UnesiKretanje(prosli))
        return 0;
    while(true) {
        Kretanje *trenutni = new Kretanje;
        if(!UnesiKretanje(trenutni))
            break;

        trenutni->sljedeci = nullptr;
        prosli->sljedeci = trenutni;
        prosli = trenutni;
    }
    // std::multimap<KretanjeBezVeze, double, Kriterij> brzine;
    std::map<KretanjeBezVeze, double, Kriterij> brzine;
    int brojElemenata = 0;
    for(Kretanje *p = pocetak; p != nullptr; p = p->sljedeci, brojElemenata++) {
        Kretanje objekat = *p;
        KretanjeBezVeze kretanje{p->put, p->pocetak, p->kraj};
        double brzina = (p->put)/(VrijemeUSekunde(p->kraj) - VrijemeUSekunde(p->pocetak));
        std::cout << "Kretanje: " << kretanje.put << ", " << kretanje.pocetak.sati << ", " << kretanje.kraj.sati << "\n";
        // brzine[kretanje] = brzina;
        brzine.insert({kretanje, brzina});
    }
    double maxBrzina = -1;
    double minBrzina = 2147000000;
    double ukupnoVrijeme = 0;
    double prosjecnaBrzina = 0;
    std::cout << "\n\n";
    for(auto &par : brzine) {
        std::cout << par.first.put << " ";
        KretanjeBezVeze kretanje = par.first;
        ukupnoVrijeme += (VrijemeUSekunde(kretanje.kraj) - VrijemeUSekunde(kretanje.pocetak));
        double brzina = par.second;
        prosjecnaBrzina += brzina;
        maxBrzina = std::max(maxBrzina, brzina);
        minBrzina = std::min(minBrzina, brzina);
    }
    std::cout << "\n\n";
    double srednjaBrzina = (maxBrzina + minBrzina) / ukupnoVrijeme;
    prosjecnaBrzina /= (double)brojElemenata;
    std::cout << "Minimalna: " << minBrzina << "\n";
    std::cout << "Maximalna: " << maxBrzina << "\n";
    std::cout << "Srednja: " << srednjaBrzina << "\n";
    std::cout << "Prosjecna: " << prosjecnaBrzina << "\n";
    return 0;
}
// 10
// 10 10 10
// 10 10 30
// 25 
// 10 10 30
// 10 10 55
// 10
// 10 10 55
// 10 11 15
