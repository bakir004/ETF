#include <cstdio>
#include <iostream>
#include <cmath>


class Ugao {
    static const double PI;
    double ugao;
    static double DajSvedenoNaDvaPi(double radijani) {
        while(radijani < 0)
            radijani += 2*PI;
        while(radijani >= 2*PI)
            radijani -= 2*PI;
        return radijani;
    }
    static double DajRadijane(int stepeni, int minute, int sekunde) {
        return (stepeni + (double)minute/60 + (double)sekunde/3600) / 180 * PI;
    }
public:
    Ugao(double radijani = 0) { Postavi(radijani); }
    Ugao(int stepeni, int minute, int sekunde) { Postavi(stepeni, minute, sekunde); }
    Ugao &Postavi(double radijani) {
        ugao = DajSvedenoNaDvaPi(radijani);
        return *this; 
    }
    Ugao &Postavi(int stepeni, int minute, int sekunde) {
        ugao = DajSvedenoNaDvaPi(DajRadijane(stepeni, minute, sekunde));
        return *this;
    }
    double DajRadijane() const { return ugao; };
    void OcitajKlasicneJedinice(int &stepeni, int &minute, int &sekunde);
    int DajStepene() const;
    int DajMinute() const;
    int DajSekunde() const;
    void Ispisi() const { std::cout << "Ugao: " << ugao << " radijana"; }
    void IspisiKlasicno() const;
    Ugao &SaberiSa(const Ugao &u);
    Ugao &PomnoziSa(double x);
    friend Ugao ZbirUglova(const Ugao &u1, const Ugao &u2);
    friend Ugao ProduktUglaSaBrojem(const Ugao &u, double x);
};

const double Ugao::PI = atan(1)*4;

void Ugao::OcitajKlasicneJedinice(int &stepeni, int &minute, int &sekunde) {
    double stepeniDecimalno = ugao * 180 / PI;
    stepeni = int(stepeniDecimalno);
    stepeniDecimalno -= stepeni;
    stepeniDecimalno *= 60;
    minute = int(stepeniDecimalno);
    stepeniDecimalno -= minute;
    stepeniDecimalno *= 60;
    sekunde = stepeniDecimalno;
}

int main() {
    Ugao u1(55, 45, 20);
    u1.Ispisi();
    int stepeni, minute, sekunde;
    u1.OcitajKlasicneJedinice(stepeni, minute, sekunde);
    std::cout << stepeni << " " << minute << " " << sekunde;
    return 0;
}
