#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <deque>
#include <stdexcept>
#include <type_traits>
#include <vector>

enum class SmjerKretanja {
    NaprijedNazad, NazadNaprijed, GoreDolje, DoljeGore, LijevoDesno, DesnoLijevo
};

template <typename Tip3DKont>
auto IzdvojiDijagonale3D(Tip3DKont kont, SmjerKretanja smjer) {
    if(kont.size() == 0) throw std::domain_error("Prazan kontejner");
    std::remove_reference_t<decltype(kont.at(0))> rezultantnaMatrica;

    // kontrola validnosti kvadra
    int visinaIliBrojRedovaMatrice = kont.at(0).size();
    if(visinaIliBrojRedovaMatrice == 0) throw std::domain_error("Prazna podmatrica");
    int sirinaIliBrojElemenataRedaMatrice = kont.at(0).at(0).size();
    if(sirinaIliBrojElemenataRedaMatrice == 0) throw std::domain_error("Prazan red");

    for(auto matrica : kont) {
        if(matrica.size() == 0) throw std::domain_error("Prazna podmatrica");
        if(matrica.size() != visinaIliBrojRedovaMatrice) throw std::domain_error("Grbav kvadar");
        for(auto red : matrica) {
            if(red.size() == 0) throw std::domain_error("Prazan red");
            if(red.size() != sirinaIliBrojElemenataRedaMatrice) throw std::domain_error("Grbava matrica");
        }
    }
    // end kontrola validnosti kvadra

    int dubina = kont.size();
    int visina = kont.at(0).size();
    int sirina = kont.at(0).at(0).size();

    // stvarno ne znam kako ovo dole generalizirati
    if(smjer == SmjerKretanja::NaprijedNazad) {
        for(int i = 0; i < dubina; i++) {
            std::remove_reference_t<decltype(kont.at(0).at(0))> dijagonala;
            for(int j = 0; j < std::min(visina, sirina); j++)
                dijagonala.push_back(kont.at(i).at(j).at(j));
            rezultantnaMatrica.push_back(dijagonala);
        }
    } else if(smjer == SmjerKretanja::NazadNaprijed) {
        for(int i = dubina - 1; i >= 0; i--) {
            std::remove_reference_t<decltype(kont.at(0).at(0))> dijagonala;
            for(int j = 0; j < visina; j++)
                for(int k = 0; k < sirina; k++)
                    if(j + k == std::max(visina, sirina) - 1)
                        dijagonala.push_back(kont.at(i).at(j).at(k));
            rezultantnaMatrica.push_back(dijagonala);
        }
    } else if(smjer == SmjerKretanja::LijevoDesno) {
        for(int k = 0; k < sirina; k++) {
            std::remove_reference_t<decltype(kont.at(0).at(0))> dijagonala;
            for(int j = 0; j < visina; j++)
                for(int i = 0; i < dubina; i++)
                    if(i + j == std::min(dubina, visina) - 1)
                        dijagonala.push_back(kont.at(i).at(j).at(k));
            rezultantnaMatrica.push_back(dijagonala);
        }
    } else if(smjer == SmjerKretanja::DesnoLijevo) {
        for(int k = sirina - 1; k >= 0; k--) {
            std::remove_reference_t<decltype(kont.at(0).at(0))> dijagonala;
            for(int i = 0; i < std::min(visina, dubina); i++)
                dijagonala.push_back(kont.at(i).at(i).at(k));
            rezultantnaMatrica.push_back(dijagonala);
        }
    } else if(smjer == SmjerKretanja::GoreDolje) {
        for(int j = 0; j < visina; j++) {
            std::remove_reference_t<decltype(kont.at(0).at(0))> dijagonala;
            for(int k = 0; k < sirina; k++)
                for(int i = 0; i < dubina; i++)
                    if(i + k == std::min(dubina, sirina) - 1)
                        dijagonala.push_back(kont.at(i).at(j).at(k));
            rezultantnaMatrica.push_back(dijagonala);
        }
    } else if(smjer == SmjerKretanja::DoljeGore) {
        for(int j = visina - 1; j >= 0; j--) {
            std::remove_reference_t<decltype(kont.at(0).at(0))> dijagonala;
            for(int i = dubina - 1; i >= 0; i--)
                for(int k = 0; k < sirina; k++) 
                    if(dubina - i - 1 + k == std::max(dubina, sirina) - 1) 
                        dijagonala.push_back(kont.at(i).at(j).at(k));
            rezultantnaMatrica.push_back(dijagonala);
        }
    } 
    
    return rezultantnaMatrica;
}


int main() {
    std::cout << "Unesite dimenzije (x y z): ";
    int x,y,z;
    std::cin >> x >> y >> z;
    std::vector<std::deque<std::deque<int>>> kontejner(x, std::deque<std::deque<int>>(y, std::deque<int>(z, 0)));
    std::cout << "Unesite elemente kontejnera: \n";
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            for(int k = 0; k < z; k++)
                std::cin >> kontejner.at(i).at(j).at(k);

    int smjer;
    std::cout << "Unesite smjer kretanja [0-5]: ";
    std::cin >> smjer;
    std::array<SmjerKretanja, 6> smjerovi{{SmjerKretanja::NaprijedNazad, SmjerKretanja::NazadNaprijed, SmjerKretanja::GoreDolje, SmjerKretanja::DoljeGore, SmjerKretanja::LijevoDesno, SmjerKretanja::DesnoLijevo}};
    std::array<std::string, 6> smjerUString{{"NaprijedNazad","NazadNaprijed","GoreDolje","DoljeGore","LijevoDesno","DesnoLijevo"}};
    try {
        auto matricaDijagonala = IzdvojiDijagonale3D(kontejner, smjerovi.at(smjer));
        std::cout << smjerUString.at(smjer) << ":\n";
        for(auto red : matricaDijagonala){
            for(auto element : red)
                std::cout << std::setw(4) << element;
            std::cout << "\n";
        }
    } catch(std::domain_error err) {
        std::cout << err.what() << "!!!";
    }
    return 0;
}