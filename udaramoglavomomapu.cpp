#include <map>
#include <iostream>

struct Vrijeme {
    int sati, minute, sekunde;
};

struct Kretanje {
    Vrijeme pocetak, kraj;
    double put;
    Kretanje *slj;
    bool operator<(const Kretanje &other) const {
        return true;
    }
};

int main() {
    std::map<Kretanje, int> mapa;
    Kretanje k1{{1,2,3},{4,5,6},1,nullptr};
    Kretanje k2{{2,3,4},{5,6,7},2,&k1};
    Kretanje k3{{1,2,3},{4,5,7},2,&k2};
    // mapa[k1] = 1;
    // mapa[k2] = 2;
    // mapa[k3] = 2;
    mapa.insert({k1, 1});
    mapa.insert({k2, 2});
    mapa.insert({k3, 3});
    for(auto par : mapa) {
        std::cout << "KOLIKO?\n";
    }
    return 0;
}
