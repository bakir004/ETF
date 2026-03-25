#include <algorithm>
#include <iostream>
#include <cmath>
#include <array>
#include <stdexcept>
#define EPSILON 1e-10

/*promjena*/
typedef std::pair<double, double> Tacka;
enum Pozicija {GoreLijevo, GoreDesno, DoljeLijevo, DoljeDesno};
enum Smjer {Nalijevo, Nadesno};

class Pravougaonik {
    Tacka t1, t2;
public:
    Pravougaonik(const Tacka &t1, const Tacka &t2) { Postavi(t1, t2); }
    bool JeTacka() const;
    void Postavi(const Tacka &t1, const Tacka &t2);
    void Postavi(Pozicija p, const Tacka &t);
    void Centriraj(const Tacka &t);
    Tacka DajTjeme(Pozicija p) const;
    Tacka DajCentar() const;
    double DajHorizontalnu() const;
    double DajVertikalnu() const;
    double DajObim() const;
    double DajPovrsinu() const;
    static Pravougaonik Presjek(const Pravougaonik &p1, const Pravougaonik &p2);
    void Transliraj(double delta_x, double delta_y);
    void Rotiraj(const Tacka &t, Smjer s);
    void Ispisi() const;
    friend bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2);
};

bool Pravougaonik::JeTacka() const { return t1 == t2; }

bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2) {
    double p1h = p1.DajHorizontalnu();
    double p1v = p1.DajVertikalnu();
    double p2h = p2.DajHorizontalnu();
    double p2v = p2.DajVertikalnu();
    if(p1.JeTacka() || p2.JeTacka()) return true;
    if((p1h == 0 || p1v == 0) && (p2h == 0 || p2v == 0) && !p1.JeTacka() && !p2.JeTacka()) return true;
    if(std::abs(p1h/p2h - p1v/p2v) < EPSILON) return true;
    Pravougaonik kopijaJednog = p2;
    kopijaJednog.Rotiraj(kopijaJednog.DajCentar(), Smjer::Nalijevo);
    p2h = kopijaJednog.DajHorizontalnu();
    p2v = kopijaJednog.DajVertikalnu();
    return std::abs(p1h/p2h - p1v/p2v) < EPSILON;
}

bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2) {
    return p1.DajHorizontalnu() == p2.DajHorizontalnu() && p1.DajVertikalnu() == p2.DajVertikalnu() ||
        p1.DajHorizontalnu() == p2.DajVertikalnu() && p1.DajVertikalnu() == p2.DajHorizontalnu();
}

bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2) {
    return p1.DajTjeme(Pozicija::GoreLijevo) == p2.DajTjeme(Pozicija::GoreLijevo)
        && p1.DajTjeme(Pozicija::DoljeDesno) == p2.DajTjeme(Pozicija::DoljeDesno);
}

void Pravougaonik::Rotiraj(const Tacka &t, Smjer s) {
    Tacka vektorPomjeraja = {t.first, t.second};
    Transliraj(-vektorPomjeraja.first, -vektorPomjeraja.second);
    std::swap(t1.first, t1.second);
    if(s == Smjer::Nalijevo) t1.first *= -1;
    else t1.second *= -1;
    std::swap(t2.first, t2.second);
    if(s == Smjer::Nalijevo) t2.first *= -1;
    else t2.second *= -1;
    Transliraj(vektorPomjeraja.first, vektorPomjeraja.second);
    Postavi(t1,t2);
}

void Pravougaonik::Transliraj(double delta_x, double delta_y) {
    t1.first += delta_x;
    t2.first += delta_x;
    t1.second += delta_y;
    t2.second += delta_y;
}

Pravougaonik Pravougaonik::Presjek(const Pravougaonik &p1, const Pravougaonik &p2) {
    if(p1.t2.first < p2.t1.first || p2.t2.first < p1.t1.first ||
            p1.t2.second > p2.t1.second || p2.t2.second > p1.t1.second)
        throw std::domain_error("Pravougaonici se ne presijecaju");
    Tacka presjekT1 = {std::max(p1.t1.first, p2.t1.first), std::max(p1.t2.second, p2.t2.second)};
    Tacka presjekT2 = {std::min(p1.t2.first, p2.t2.first), std::min(p1.t1.second, p2.t1.second)};
    Pravougaonik presjek(presjekT1, presjekT2);
    return presjek;
}

double Pravougaonik::DajPovrsinu() const { return DajHorizontalnu() * DajVertikalnu(); }
double Pravougaonik::DajObim() const { return 2 * DajHorizontalnu() + 2 * DajVertikalnu(); }
double Pravougaonik::DajVertikalnu() const { return std::abs(t1.second - t2.second); }
double Pravougaonik::DajHorizontalnu() const { return std::abs(t1.first - t2.first); }
Tacka Pravougaonik::DajCentar() const { return {(t1.first + t2.first) / 2, (t1.second + t2.second) / 2}; }

Tacka Pravougaonik::DajTjeme(Pozicija p) const {
    std::array<Tacka, 4> tjemena{{ 
        t1, {std::max(t1.first,t2.first),std::max(t1.second,t2.second)},
        {std::min(t1.first,t2.first),std::min(t1.second,t2.second)}, t2
    }};
    return tjemena[(int)p];
}

void Pravougaonik::Centriraj(const Tacka &t) {
    Tacka trenutniCentar = DajCentar();
    Tacka vektorPomjeraja = {t.first-trenutniCentar.first, t.second-trenutniCentar.second};
    t1.first += vektorPomjeraja.first;
    t1.second += vektorPomjeraja.second;
    t2.first += vektorPomjeraja.first;
    t2.second += vektorPomjeraja.second;
}

void Pravougaonik::Postavi(Pozicija p, const Tacka &t) {
    Tacka pivot;
    if(p == Pozicija::GoreLijevo) pivot = t2;
    else if(p == Pozicija::DoljeDesno) pivot = t1;
    else if(p == Pozicija::GoreDesno) pivot = DajTjeme(Pozicija::DoljeLijevo);
    else if(p == Pozicija::DoljeLijevo) pivot = DajTjeme(Pozicija::GoreDesno);
    Postavi(pivot, t);
}

void Pravougaonik::Postavi(const Tacka &t1, const Tacka &t2) {
    Tacka t1Kopija = t1;
    Tacka t2Kopija = t2;
    this->t1.first = std::min(t1Kopija.first, t2Kopija.first);
    this->t1.second = std::max(t1Kopija.second, t2Kopija.second);
    this->t2.first = std::max(t1Kopija.first, t2Kopija.first);
    this->t2.second = std::min(t1Kopija.second, t2Kopija.second);
}

void Pravougaonik::Ispisi() const {
    std::cout << "{{" << t1.first << "," << t1.second << "},{" << t2.first << "," << t2.second << "}}";
}

int main() {
    int n;
    std::cout << "Unesite n: ";
    std::cin >> n;
    Pravougaonik **pravougaonici = nullptr;
    try {
        pravougaonici = new Pravougaonik*[n]{};
        Tacka t1,t2;
        for(int i = 0; i < n; i++) {
            std::cout << "Unesite 1. tjeme pravougaonika " << i+1 << ": ";
            std::cin >> t1.first >> t1.second;
            std::cout << "Unesite 2. tjeme pravougaonika " << i+1 << ": ";
            std::cin >> t2.first >> t2.second;
            pravougaonici[i] = new Pravougaonik(t1,t2);
        }
        std::cout << "Unesite podatke za transliranje (dx dy): ";
        double deltaX, deltaY;
        std::cin >> deltaX >> deltaY;
        std::transform(pravougaonici, pravougaonici+n, pravougaonici, [deltaX, deltaY](Pravougaonik* p){
                p->Transliraj(deltaX, deltaY);
                p->Rotiraj(p->DajCentar(), Smjer::Nalijevo);
                return p;
                });
        std::cout << "Pravougaonici, nakon transformacija: \n";
        std::for_each(pravougaonici, pravougaonici+n, [](Pravougaonik* p){
                p->Ispisi();
                std::cout << "\n";
                });
        Pravougaonik najveci = **std::max_element(pravougaonici, pravougaonici+n, [](Pravougaonik* p1, Pravougaonik* p2){
                return p1->DajPovrsinu() < p2->DajPovrsinu();
                });
        std::cout << "Pravougaonik s najvecom povrsinom: ";
        najveci.Ispisi();
    } catch(...) {
        std::cout << "Problemi s memorijom";
    }
    for(int i = 0; i < n; i++)
        delete pravougaonici[i];
    delete[] pravougaonici;
    return 0;
}
