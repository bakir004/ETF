#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

class Kugla {
    static const double PI;
    static const double EPSILON;
    double r;
    double x,y,z;
public: 
    explicit Kugla(double r = 0): r(r) {}
    Kugla(double x, double y, double z, double r = 0): x(x), y(y), z(z), r(r) {}
    explicit Kugla(const std::tuple<double, double, double> &centar, double r = 0) {
        x = std::get<0>(centar);
        y = std::get<1>(centar);
        z = std::get<2>(centar);
        this->r = r;
    }
    double DajX() const { return x; }
    double DajY() const { return y; }
    double DajZ() const { return z; }
    std::tuple<double, double, double> DajCentar() const { return std::make_tuple(x,y,z); }
    double DajPoluprecnik() const { return r; };
    double DajPovrsinu() const { return 4 * r * r * PI; };
    double DajZapreminu() const { return 4./3 * r * r * r * PI; };
    Kugla &PostaviX(double x) { this->x = x; return *this; }
    Kugla &PostaviY(double y) { this->y = y; return *this; }
    Kugla &PostaviZ(double z) { this->z = z; return *this; }
    Kugla &PostaviCentar(double x, double y, double z) { this->x = x; this->y = y; this->z = z; return *this; }
    Kugla &PostaviCentar(const std::tuple<double, double, double> &centar) {
        x = std::get<0>(centar);
        y = std::get<1>(centar);
        z = std::get<2>(centar);
        return *this;
    }
    Kugla &PostaviPoluprecnik(double r) { this->r = r; return *this; }
    void Ispisi() const { std::cout << "{(" << x << "," << y << "," << z << ")," << r << "}"; };
    void Transliraj(double delta_x, double delta_y, double delta_z) { x += delta_x; y += delta_y; z += delta_z; }
    friend bool DaLiSuIdenticne(const Kugla &k1, const Kugla &k2);
    friend bool DaLiSuPodudarne(const Kugla &k1, const Kugla &k2);
    friend bool DaLiSuKoncentricne(const Kugla &k1, const Kugla &k2);
    friend bool DaLiSeDodirujuIzvana(const Kugla &k1, const Kugla &k2);
    friend bool DaLiSeDodirujuIznutra(const Kugla &k1, const Kugla &k2);
    friend bool DaLiSePreklapaju(const Kugla &k1, const Kugla &k2);
    friend bool DaLiSeSijeku(const Kugla &k1, const Kugla &k2);
    friend double Udaljenost(const Kugla &k1, const Kugla &k2);
    bool DaLiSadrzi(const Kugla &k) const;
    friend double RastojanjeCentara(const Kugla &k1, const Kugla &k2);
};

const double Kugla::PI = std::atan(1) * 4;
const double Kugla::EPSILON = 1e-10;

double Udaljenost(const Kugla &k1, const Kugla &k2) {
    return std::sqrt(pow(k1.DajX() - k2.DajX(), 2) + pow(k1.DajZ() - k2.DajZ(), 2) + pow(k1.DajZ() - k2.DajZ(), 2));
}
bool DaLiSuIdenticne(const Kugla &k1, const Kugla &k2) {
    return k1.DajCentar() == k2.DajCentar() && k1.DajPoluprecnik() == k2.DajPoluprecnik();
}
bool DaLiSuPodudarne(const Kugla &k1, const Kugla &k2) {
    return k1.DajPoluprecnik() == k2.DajPoluprecnik();
}
bool DaLiSuKoncentricne(const Kugla &k1, const Kugla &k2) {
    return k1.DajCentar() == k2.DajCentar();
}
bool DaLiSeDodirujuIzvana(const Kugla &k1, const Kugla &k2) {
    return std::abs(Udaljenost(k1, k2) - k1.DajPoluprecnik() - k2.DajPoluprecnik()) < Kugla::EPSILON;
}
bool DaLiSeDodirujuIznutra(const Kugla &k1, const Kugla &k2) {
    return std::abs(Udaljenost(k1, k2) + k1.DajPoluprecnik() - k2.DajPoluprecnik()) < Kugla::EPSILON
        || std::abs(Udaljenost(k1, k2) + k2.DajPoluprecnik() - k1.DajPoluprecnik()) < Kugla::EPSILON;
}
bool DaLiSePreklapaju(const Kugla &k1, const Kugla &k2) {
    return Udaljenost(k1, k2) + Kugla::EPSILON < k1.DajPoluprecnik() + k2.DajPoluprecnik();
}
bool DaLiSeSijeku(const Kugla &k1, const Kugla &k2) {
    return Udaljenost(k1, k2) < k1.DajPoluprecnik() + k2.DajPoluprecnik();
}
bool Kugla::DaLiSadrzi(const Kugla &k) const {
    return std::abs(Udaljenost(*this, k) + k.DajPoluprecnik()) - Kugla::r < EPSILON;
}

int main() {
    Kugla k1({0,0,0},5), k2({7,0,0},2), k3({3,0,0},2);
    std::cout << DaLiSeSijeku(k1, k2);
    std::cout << k1.DaLiSadrzi(k3);
    int n;
    std::cin >> n;
    std::vector<std::shared_ptr<Kugla>> pokazivaci(n);
    for(int i = 0; i < n; i++) {
        int x,y,z,r;
        std::cin >> x >> y >> z >> r;
        pokazivaci[i] = std::make_shared<Kugla>(new Kugla({x,y,z},r));
    }
    return 0;
}
