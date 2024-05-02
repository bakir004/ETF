#include <vector>
#include <iostream>

struct Koordinata {
    double x, y;
    Koordinata *sljedeci;
};

Koordinata *KreirajListu(std::vector<std::pair<double, double>> koordinate) {
    Koordinata *pocetak = new Koordinata;
    Koordinata *prosli = pocetak;
    prosli->x = koordinate[0].first;
    prosli->y = koordinate[0].second;
    try {
        for(int i = 1; i < koordinate.size(); i++) {
            Koordinata *novi = new Koordinata;
            novi->x = koordinate[i].first;
            novi->y = koordinate[i].second;
            novi->sljedeci = nullptr;
            prosli->sljedeci = novi;
            prosli = novi;
        }
    } catch(...) {
        // ObrisiListu(pocetak)
        throw;
    }
    prosli->sljedeci = pocetak;
    return pocetak;
}

int main() {
    Koordinata *pocetak = KreirajListu({{1,2},{3,4},{5,6},{7,8},{9,10}});
    for(int i = 0; i < 20; i++) {
        std::cout << pocetak->x << "," << pocetak->y << "\n";
        pocetak = pocetak->sljedeci;
    }
    // ObrisiListu(pocetak)
    return 0;
}
