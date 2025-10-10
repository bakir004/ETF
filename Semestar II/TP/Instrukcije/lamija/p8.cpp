#include<iostream>
#include<unordered_map>
#include<utility>

int main() {
    std::pair<int, std::string> tacka(2,"bakir");
    // tacka = std::make_pair(5, "ibro peskun");
    tacka = {5, "lamija"};
    // std::cin >> tacka.first >> tacka.second;
    // std::cout << tacka.first << ", " << tacka.second << std::endl;

    // std::unordered_map<int, std::string> studenti;
    //
    // studenti[20000] = "Meho Cinjarevic";
    // studenti[10000] = "Mujo Cinjarevic";
    // studenti[19705] = "Bakir Cinjarevic";
    //
    // for(auto par : studenti) {
    //     std::cout << par.first << ": " << par.second << std::endl;
    // }

    // std::cout << studenti[19707];
    //
    // std::map<std::string, std::vector<std::string>> povezanostGradova;
    //
    // povezanostGradova["Sarajevo"] = {"Travnik", "Zenica", "Mostar"};
    // povezanostGradova["Mostar"] = {"Rudo", "Neum", "Trebinje"};



    return 0;
}
