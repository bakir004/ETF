// TP 2023/2024: Zadaća 3, Zadatak 2
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::map<std::string, std::set<std::tuple<std::string, int, int>>> Index;
typedef std::set<std::tuple<std::string, int, int>> Pretraga;

Index KreirajIndeksPojmova(Knjiga a) {
    int br_stranice = 1;

    std::map<std::string, bool> obradjene_recenice;

    Index indeks;

    for (auto par : a) {
        std::string poglavlje = par.first;
        std::vector<std::string> sadrzaj = par.second;

        for (int i = 0; i < sadrzaj.size(); i++, ++br_stranice) {
            std::string recenica = sadrzaj.at(i);
            auto pocetak = recenica.begin();
            auto kraj = recenica.end();

            std::vector<std::string> rijeci;
            std::vector<std::string> recenice;

            std::vector<int> pojavljivanja;

            while (pocetak != recenica.end()) {
                while (!std::isalnum(*pocetak) && pocetak != recenica.end())
                    pocetak++;
                if(pocetak == recenica.end()) break;
                auto kraj_rijeci = std::find_if( pocetak, recenica.end(), [](char x) { return (!std::isalnum(x)); });
                auto vel = std::string(pocetak, kraj_rijeci);
                for (auto &x : vel)
                    x = std::toupper(x);
                rijeci.push_back(vel);
                pojavljivanja.push_back(pocetak - recenica.begin() + 1);
                pocetak = (kraj_rijeci != recenica.end()) ? kraj_rijeci + 1 : recenica.end();
            }

            // for (auto x : rijeci)
            //     std::cout << "" << x << " ";
            // std::cout << std::endl << std::endl;
            //
            // for(auto y : pojavljivanja) std::cout << y << " ";
            //   std::cout << std::endl << std::endl;

            recenice.push_back(recenica);

            for(int i = 0; i < rijeci.size(); i++) {
                indeks[rijeci[i]].insert(std::make_tuple(poglavlje, br_stranice, pojavljivanja[i]));
            }           

            // nalazenje rijeci
        //     std::map<std::string, bool> obradjeno;
        //     for (const auto &recenica : recenice) {
        //         if (obradjene_recenice.find(recenica) != obradjene_recenice.end())
        //             obradjene_recenice[recenica] = true;
        //         else
        //             obradjene_recenice.insert({recenica, true});
        //
        //         for (int i = 0; i < rijeci.size(); i++) {
        //             if (obradjeno.find(rijeci.at(i)) != obradjeno.end())
        //                 continue;
        //
        //             obradjeno[rijeci.at(i)] = true;
        //
        //             // pushanje
        //             int pozicija = 0;
        //             while (pozicija < recenica.length()) {
        //                 pozicija = recenica.find(rijeci.at(i), pozicija);
        //                 if (pozicija >= recenica.length())
        //                     break;
        //
        //                 indeks[rijeci.at(i)].insert(
        //                         std::make_tuple(par.first, br_stranice, pozicija + 1));
        //                 pozicija += rijeci.at(i).size() + 1;
        //             }
        //         }
        //     }
        }
    }

    return indeks;
}

Pretraga PretraziIndeksPojmova(std::string rijec, const Index &indeks) {

    // std::transform(rijec.begin(), rijec.end(), rijec.begin(), :toupp:er);

    /*auto kriterij_velika_slova = [](char c) {
      return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
      };*/

    std::transform(rijec.begin(), rijec.end(), rijec.begin(), ::toupper);

    // provjeravamo da li rijec ima samo slova
    for (char a : rijec) {
        if(!std::isalnum(a))
            throw std::domain_error("Neispravna rijec");
    }

    // Index index_velika_slova;
    // for (const auto &p : indeks) {
    //     std::string velika_slova = p.first;
    //     std::transform(velika_slova.begin(), velika_slova.end(),
    //             velika_slova.begin(), ::toupper);
    //     index_velika_slova[velika_slova] = p.second;
    // }

    auto it = indeks.find(rijec);

    if (it == indeks.end())
        return Pretraga{};

    return Pretraga(it->second.begin(), it->second.end());
}

void IspisiIndeksPojmova(const Index &indeks) {

    for (const auto &p : indeks) {

        std::string velika_slova = p.first;
        std::transform(velika_slova.begin(), velika_slova.end(),
                velika_slova.begin(), ::toupper);

        std::cout << velika_slova << ": ";

        int i = 0;
        for (const auto &t : p.second) {
            std::cout << std::get<0>(t) << "/" << std::get<1>(t) << "/"
                << std::get<2>(t);

            if (i < p.second.size() - 1)
                std::cout << ", ";
            i++;
        }

        std::cout << std::endl;
    }
}

int main() {

    Knjiga knjiga;
    int i = 1;

    while (true) {
        std::string unos;
        std::string poglavlje;

        std::cout << "Unesite naziv poglavlja: " << std::endl;
        std::getline(std::cin, poglavlje);
        i = 1;
        if (poglavlje.empty())
            break;

        knjiga.insert({"pomocni", {}});

        while (true) {
            std::cout << "Unesite sadrzaj stranice " << i << ": " << std::endl;
            i++;
            std::getline(std::cin, unos);
            if (unos.empty())
                break;
            knjiga[poglavlje].push_back(unos);
        }
    }

    auto index = KreirajIndeksPojmova(knjiga);
    std::cout << "\nKreirani indeks pojmova: " << std::endl;
    IspisiIndeksPojmova(index);

    std::cout << std::endl;

    while (true) {
        std::string trazi;
        std::cout << "Unesite rijec: " << std::endl;
        std::getline(std::cin, trazi);

        if (trazi.empty()) {
            std::cout << "Dovidjenja!";
            break;
        }

        try {
            auto pozicije = PretraziIndeksPojmova(trazi, index);

            if (pozicije.empty())
                std::cout << "Rijec nije nadjena!";
            else {
                std::cout << "Rijec nadjena na pozicijama: ";
                for (const auto &x : pozicije)
                    std::cout << std::get<0>(x) << "/" << std::get<1>(x) << "/"
                        << std::get<2>(x) << " ";
            }
            std::cout << std::endl;
        } catch (std::domain_error &e) {
            std::cout << e.what()<< "!" << std::endl;
        }
    }

    // Knjiga Nova = {{"I", {"Pazljivo procitati zadatak!"}},
    //     {"II", {"Izdvojiti lagano zahtjeve."}},
    //     {"III", {"Napisati kod PAZLJIVO :)"}},
    //     {"IV", {"Lagano ubrati sve bodove ;)"}}};
    // std::map<std::string, std::set<std::tuple<std::string, int, int>>>
    //     indeksi_pojmova{KreirajIndeksPojmova(Nova)};
    // IspisiIndeksPojmova(indeksi_pojmova);
    // try {
    //     std::set<std::tuple<std::string, int, int>> pozicije1{
    //         PretraziIndeksPojmova("lAgAnO", indeksi_pojmova)};
    //     std::cout << "Rijec se pojavljuje " << pozicije1.size() << "x u knjizi."
    //         << std::endl;
    //     std::set<std::tuple<std::string, int, int>> pozicije2{
    //         PretraziIndeksPojmova("pazLJiVo", indeksi_pojmova)};
    //     std::cout << "Rijec se pojavljuje " << pozicije2.size() << "x u knjizi."
    //         << std::endl;
    //     std::set<std::tuple<std::string, int, int>> pozicije3{
    //         PretraziIndeksPojmova(":)", indeksi_pojmova)};
    //     std::cout << "Rijec se pojavljuje " << pozicije3.size() << "x u knjizi."
    //         << std::endl;
    // } catch (std::domain_error &e) {
    //     std::cout << "IZUZETAK: " << e.what() << "!" << std::endl;
    // }

    return 0;
}
