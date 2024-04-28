#include <array>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define BROJ_PRAVACA 8

namespace Robot {
    enum class Pravci { Sjever, Sjeveroistok, Istok, Jugoistok, Jug, Jugozapad, Zapad, Sjeverozapad };
    enum class KodoviGresaka { PogresnaKomanda, NedostajeParametar, SuvisanParametar, NeispravanParametar };
    enum class Komande { Idi, Rotiraj, Sakrij, Otkrij, PrikaziTeren, Kraj };
    int xmin = -10, xmax = 10, ymin = -10, ymax = 10;
    bool vidljivost = true, bioVidljiv = true;
    std::array<std::vector<int>, 8> vektoriPravca{{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}};
    std::array<std::string, 8> stringoviPravaca{{"sjever", "sjeveroistok", "istok", "jugoistok", "jug", "jugozapad", "zapad", "sjeverozapad"}};
    std::array<std::string, 4> stringoviKodovaGresaka{ {"Nerazumljiva komanda!\n", "Komanda trazi parametar koji nije naveden!\n", "Zadan je suvisan parametar nakon komande!\n", "Parametar komande nije ispravan!\n"}};
    std::array<std::string, 2> stringoviVidljivosti{{"nevidljiv", "vidljiv"}};
    void KreirajTeren(int xmin, int xmax, int ymin, int ymax);
    bool Idi(int &x, int &y, Pravci orijentacija, int korak);
    bool UGranicamaTerena(int x, int y);
    void Rotiraj(Pravci &orijentacija, int ugao);
    void Sakrij();
    void Otkrij();
    void IspisiPoziciju(int x, int y, Pravci orijentacija);
    void PrikaziTeren();
    void PrijaviGresku(KodoviGresaka kod_greske);
    bool IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y,
            Pravci &orijentacija);
    bool UnosKomande(Robot::Komande &komanda, int &parametar,
            Robot::KodoviGresaka &kod_greske);

    std::vector<std::vector<char>> matricaTerena;
} 

int signum(int x) { return (x > 0) - (x < 0); }

bool Robot::UGranicamaTerena(int x, int y) { return (x >= xmin && x <= xmax) && (y >= ymin && y <= ymax); }

void Robot::KreirajTeren(int noviXMin, int noviXMax, int noviYMin, int noviYMax) { 
    if (noviXMin >= noviXMax || noviYMin >= noviYMax) throw std::range_error("Nelegalan opseg");
    xmin = noviXMin; xmax = noviXMax; ymin = noviYMin; ymax = noviYMax;
    matricaTerena = std::move(std::vector<std::vector<char>>( ymax - ymin + 1, std::vector<char>(xmax - xmin + 1, ' ')));
    matricaTerena.at(abs(ymax - (ymax + ymin) / 2)).at(abs(xmin) + (xmin + xmax) / 2) = 'O';
}

bool Robot::Idi(int &x, int &y, Pravci orijentacija, int korak) {
    int indexOrijentacije = static_cast<int>(orijentacija);
    std::vector<int> matematickiVektorKretanja = vektoriPravca.at(indexOrijentacije);
    int znakKoraka = signum(korak);
    matematickiVektorKretanja.at(0) *= znakKoraka;
    matematickiVektorKretanja.at(1) *= znakKoraka;
    korak *= znakKoraka;

    matricaTerena.at(abs(ymax - y)).at(abs(xmin) + x) = (!bioVidljiv && !vidljivost ? ' ' : '*');
    bioVidljiv = vidljivost;
    while (korak) {
        if (!UGranicamaTerena(x + matematickiVektorKretanja.at(0), y + matematickiVektorKretanja.at(1))) {
            std::cout << "Robot je pokusao napustiti teren!\n";
            matricaTerena.at(abs(ymax - y)).at(abs(xmin) + x) = 'O';
            return false;
        }
        x += matematickiVektorKretanja.at(0);
        y += matematickiVektorKretanja.at(1);
        if (vidljivost) matricaTerena.at(abs(ymax - y)).at(abs(xmin) + x) = '*';
        korak--;
    }
    matricaTerena.at(abs(ymax - y)).at(abs(xmin) + x) = 'O';
    return true;
}

void Robot::Rotiraj(Pravci &orijentacija, int ugao) {
    ugao %= BROJ_PRAVACA;
    int indexOrijentacije = static_cast<int>(orijentacija);
    int noviIndexOrijentacije = (indexOrijentacije + BROJ_PRAVACA - ugao) % BROJ_PRAVACA;
    orijentacija = Pravci(noviIndexOrijentacije);
}

void Robot::Sakrij() { vidljivost = false; }

void Robot::Otkrij() { vidljivost = true; }

void Robot::IspisiPoziciju(int x, int y, Pravci orijentacija) {
    std::cout << "Robot je " << (vidljivost ? "vidljiv" : "nevidljiv") << ", nalazi se na poziciji (" << x << "," << y << ")";
    std::cout << " i gleda na " << stringoviPravaca.at(static_cast<int>(orijentacija)) << ".\n";
}

void Robot::PrikaziTeren() {
    std::cout << std::string(xmax - xmin + 3, '#') << std::endl;
    for (int i = 0; i < ymax - ymin + 1; i++) {
        std::cout << "#";
        for (int j = 0; j < xmax - xmin + 1; j++)
            std::cout << matricaTerena.at(i).at(j);
        std::cout << "#\n";
    }
    std::cout << std::string(xmax - xmin + 3, '#') << std::endl;
}

void Robot::PrijaviGresku(KodoviGresaka kod_greske) {
    std::cout << stringoviKodovaGresaka.at(static_cast<int>(kod_greske));
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool Robot::IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y, Pravci &orijentacija) {
    bool uspjesnoProhodao = true;
    switch (komanda) {
        case Komande::Kraj: break;
        case Komande::PrikaziTeren: PrikaziTeren();
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            return true;
        case Komande::Otkrij: Otkrij(); break;
        case Komande::Sakrij: Sakrij(); break;
        case Komande::Rotiraj: Rotiraj(orijentacija, parametar); break;
        case Komande::Idi: uspjesnoProhodao = Idi(x, y, orijentacija, parametar); break;
    }
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    IspisiPoziciju(x, y, orijentacija);
    return uspjesnoProhodao; 
}

bool Robot::UnosKomande(Robot::Komande &komanda, int &parametar, Robot::KodoviGresaka &kod_greske) {
    char c;
    int n = 0;
    std::cout << "Unesi komandu: ";
    std::cin >> c;
    if (c == 'I' || c == 'R') {
        if (std::cin.peek() == '\n')
            return kod_greske = KodoviGresaka::NedostajeParametar, false;
        if (!(std::cin >> n))
            return kod_greske = KodoviGresaka::NeispravanParametar, false;
        if (std::isgraph(std::cin.peek()))
            return kod_greske = KodoviGresaka::NeispravanParametar, false;
        while (std::cin.peek() != '\n' && std::cin.get(c)) {
            if (std::isgraph(c)) {
                return kod_greske = KodoviGresaka::SuvisanParametar, false;
                break;
            }
        }
        komanda = c == 'I' ? Komande::Idi : Komande::Rotiraj;
    } else if (c == 'S') {
        if (std::cin.peek() == '\n')
            return kod_greske = KodoviGresaka::NedostajeParametar, false;
        if (!(std::cin >> c) || (std::cin.peek() != '\n' && std::cin.peek() != ' '))
            return kod_greske = KodoviGresaka::SuvisanParametar, false;
        if (c != '+' && c != '-')
            return kod_greske = KodoviGresaka::PogresnaKomanda, false;
        if (std::cin.peek() != '\n' && std::cin.peek() != ' ')
            return kod_greske = KodoviGresaka::SuvisanParametar, false;
        komanda = c == '+' ? Komande::Sakrij : Komande::Otkrij;
    } else if (c == 'T' || c == 'K') {
        if (std::cin.peek() != '\n')
            return kod_greske = KodoviGresaka::SuvisanParametar, false;
        komanda = c == 'T' ? Komande::PrikaziTeren : Komande::Kraj;
    } else
        return kod_greske = KodoviGresaka::PogresnaKomanda, false;
    return parametar = n, true;
}

int main() {
    // 14 ifova, 6 caseova, 5 ternarnih = 25 uslova
    int xmin, xmax, ymin, ymax;
    std::cout << "Unesite dimenzije terena (xmin xmax ymin ymax): ";
    std::cin >> xmin >> xmax >> ymin >> ymax;
    if (!std::cin) {
        std::cout << "Nije moguce kreirati takav teren!";
        return 0;
    }
    int x = (xmin + xmax) / 2, y = (ymax + ymin) / 2;
    Robot::Pravci orijentacija = Robot::Pravci::Sjever;
    Robot::Komande komanda;
    Robot::KodoviGresaka kod;
    int parametar;
    Robot::KreirajTeren(xmin, xmax, ymin, ymax);
    Robot::IspisiPoziciju(x, y, orijentacija);
    while (true) {
        bool uspjesno = Robot::UnosKomande(komanda, parametar, kod);
        if (komanda == Robot::Komande::Kraj)
            break;
        if (uspjesno) {
            Robot::IzvrsiKomandu(komanda, parametar, x, y, orijentacija);
        }
        else
            Robot::PrijaviGresku(kod);
    }
    std::cout << "Dovidjenja!";

    return 0;
}
