#include <iostream>
#include <stdexcept>
#include <vector>

struct Krug {
    double x, y, r;
    void Ispisi() {
        std::cout << "(" << x << "," << y << "), r = " << r << '\n';
    }
private:
    int g;
};

class Vektor {
    double x,y,z;
public:
    static Vektor Zbir(Vektor v1, Vektor v2) {
        return Vektor(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    const static int maksimalanBrojVektora;
    static int brojVektora;
    Vektor(): x(0), y(0), z(0) { 
        brojVektora++;
        if(brojVektora > maksimalanBrojVektora) throw std::domain_error("PREVISE VEKTORA");
    }
    explicit Vektor(int n): x(n), y(n), z(n) {
        brojVektora++;
        if(brojVektora > maksimalanBrojVektora) throw std::domain_error("PREVISE VEKTORA");
    }
    explicit Vektor(int x, int y, int z): x(x), y(y), z(z) {
        brojVektora++;
        if(brojVektora > maksimalanBrojVektora) throw std::domain_error("PREVISE VEKTORA");
        // *this. je ekvivalent this->
    }
    void Ispisi() const { 
        std::cout << "(" << x << "," << y << "," << z << ")\n";
    }
    // int DajX() { return x; }
    // int DajY() { return y; }
    // int DajZ() { return z; }
    void Postavi(int x, int y, int z);
    Vektor& PomnoziSa(int n);
    Vektor& SaberiSa(int n);
    friend Vektor operator+(const Vektor& v1, const Vektor& v2);
    friend int operator*(const Vektor& v1, const Vektor& v2);
    // Vektor operator+=(int n);
    Vektor& operator+=(int n);
    friend Vektor operator*(const Vektor& v1, int n);
    friend Vektor operator*(int n, const Vektor& v1);
};
const int Vektor::maksimalanBrojVektora = 10;
int Vektor::brojVektora = 0;

Vektor& Vektor::SaberiSa(int n) {
    this->x += n;
    this->y += n;
    this->z += n;
    return *this;
}
Vektor& Vektor::PomnoziSa(int n) {
    this->x *= n;
    this->y *= n;
    this->z *= n;
    return *this;
}
// Vektor Vektor::operator+=(int n);

void Vektor::Postavi(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vektor operator+(const Vektor& v1, const Vektor& v2) {
    return Vektor(v1.x + v2.x,v1.y + v2.y,v1.z + v2.z);
}
int operator*(const Vektor& v1, const Vektor& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vektor operator*(const Vektor& v1, int n) {
    return Vektor(v1.x * n, v1.y * n, v1.z * n);
}
Vektor operator*(int n, const Vektor& v1) {
    return Vektor(v1.x * n, v1.y * n, v1.z * n);
}
// Vektor operator%(Vektor v1, Vektor v2) {
//     // implementicija vektorskog proizvoda
// }

Vektor& Vektor::operator+=(int n) {
    this->x += n;
    this->y += n;
    this->z += n;
    return *this;
}

int main() {
    // Krug k1;
    // k1.x = 5;
    // k1.y = 5;
    // k1.r = -1.5;
    //
    // Vektor v1;
    // v1.Ispisi();
    // Vektor v2 = Vektor(5);
    // v2.Ispisi();
    // Vektor v3 = Vektor(1,2,3);
    // v3.Ispisi();
    // v3.Postavi(4,5,6);
    // v3.Ispisi();
    //
    // v3.Postavi(3,4,5);
    // const Vektor v4(1,2,3);
    // v4.Ispisi();
    // std::cout << v4.pi;

    // Vektor v1(1,2,1);
    // Vektor v2(2,3,2);
    // // v1.PomnoziSa(2).SaberiSa(2).Ispisi();
    // // v1.Ispisi();
    // Vektor v3 = v1 + Vektor(1,2,3);
    // v3.Ispisi();
    // int skalarni_proizvod = v1 * v2;
    // std::cout << skalarni_proizvod;


    // Vektor v1(1,2,3);
    // (v1 += 4) += 4;
    // v1.Ispisi();

    Vektor v1(1,2,3);
    Vektor v2 = 3 * v1;
    v2.Ispisi();
    v1 += 3;
    // 3 += v1;


    return 0;
}
