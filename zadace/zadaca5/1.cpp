#include <initializer_list>
#include <iostream>
#include <limits.h>
#include <stdexcept>

typedef long long int Long;
class Razlomak {
    Long brojnik, nazivnik;
    void Postavi(Long a, Long b);
    void Postavi(const Razlomak& drugi);
    short signum(Long x) const noexcept { return (x>0)-(x<0); }
public:
    static void TestirajPrekoracenjeProizvoda(Long a, Long b);
    static void TestirajPrekoracenjeSume(Long a, Long b);
    static Long NZD(Long p, Long q) noexcept {
        if(q == 0) return p;
        return NZD(q, p % q);
    }
    Razlomak(Long brojnik = 0, Long nazivnik = 1); 
    Razlomak(std::initializer_list<Long> list);
    void Ispisi() const noexcept;
    Long DajNazivnik() const noexcept { return nazivnik; }
    Long DajBrojnik() const noexcept { return brojnik; }
    friend Razlomak operator+(const Razlomak& r1, const Razlomak& r2);
    friend Razlomak operator-(const Razlomak& r1, const Razlomak& r2);
    friend Razlomak operator*(const Razlomak& r1, const Razlomak& r2);
    friend Razlomak operator/(const Razlomak& r1, const Razlomak& r2);
    friend Razlomak operator+(const Razlomak& r);
    friend Razlomak operator-(const Razlomak& r);
    Razlomak& operator+=(const Razlomak& drugi);
    Razlomak& operator-=(const Razlomak& drugi);
    Razlomak& operator*=(const Razlomak& drugi);
    Razlomak& operator/=(const Razlomak& drugi);
};
void Razlomak::Postavi(Long a = 0, Long b = 1) {
    // std::cout << "Postavljam " << a << " i " << b << "\n";
    if(b == 0) throw std::logic_error("Nekorektan razlomak");
    Long nzd = NZD(a, b);
    // std::cout << "NZD: " << nzd << "\n";
    brojnik = a / nzd * signum(b);
    nazivnik = b / nzd * signum(b);
}
void Razlomak::Postavi(const Razlomak& drugi) {
    // Long a = drugi.brojnik, b = drugi.nazivnik;
    // Long nzd = NZD(a, b);
    // brojnik = a / nzd * signum(b);
    // nazivnik = b / nzd * signum(b);
    brojnik = drugi.brojnik;
    nazivnik = drugi.nazivnik;
    if(drugi.nazivnik == 0) std::cout << "PANIKA\n";
}
void Razlomak::Ispisi() const noexcept {
    if(nazivnik == 1) std::cout << brojnik;
    else std::cout << brojnik << '/' << nazivnik;
}
Razlomak::Razlomak(std::initializer_list<Long> list) {
    if(list.size() == 1) Postavi(*list.begin());
    else if(list.size() == 2) Postavi(*list.begin(), *(list.begin()+1)); 
    else throw std::domain_error("Neispravni parametri (inicijalizacijska lista)");
}
Razlomak::Razlomak(Long brojnik, Long nazivnik)  { Postavi(brojnik, nazivnik); }

Razlomak& Razlomak::operator+=(const Razlomak& drugi) {
    Postavi(*this + drugi);
    return *this;
}
Razlomak& Razlomak::operator-=(const Razlomak& drugi) {
    Postavi(*this - drugi);
    return *this;
}
Razlomak& Razlomak::operator*=(const Razlomak& drugi) {
    Postavi(*this * drugi);
    return *this;
}
Razlomak& Razlomak::operator/=(const Razlomak& drugi) {
    Postavi(*this / drugi);
    return *this;
}
Razlomak operator+(const Razlomak& r) { return r; }
Razlomak operator-(const Razlomak& r) { return {-r.brojnik, r.nazivnik}; }

void Razlomak::TestirajPrekoracenjeSume(Long a, Long b) {
    // std::cout << "Testiram sumu " << a << " i " << b << "\n";
    if(LONG_LONG_MAX - std::abs(a) < std::abs(b)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}
void Razlomak::TestirajPrekoracenjeProizvoda(Long a, Long b) {
    // std::cout << "Testiram proizvod " << a << " i " << b << "\n";
    if(LONG_LONG_MAX / std::abs(a) < std::abs(b)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}
Razlomak operator*(const Razlomak& r1, const Razlomak& r2) {
    Long s = Razlomak::NZD(r1.brojnik, r2.nazivnik);
    Long t = Razlomak::NZD(r2.brojnik, r1.nazivnik);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.brojnik/s, r2.brojnik/t);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.nazivnik/t, r2.nazivnik/s);
    return {r1.brojnik/s * r2.brojnik/t, r1.nazivnik/t * r2.nazivnik/s};
}
Razlomak operator/(const Razlomak& r1, const Razlomak& r2) {
    Long r = Razlomak::NZD(r1.nazivnik, r2.nazivnik);
    Long u = Razlomak::NZD(r1.brojnik, r2.brojnik);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.brojnik/u, r2.nazivnik/r);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.nazivnik/r, r2.brojnik/u);
    return {r1.brojnik/u * r2.nazivnik/r, r1.nazivnik/r * r2.brojnik/u};
}
Razlomak operator+(const Razlomak& r1, const Razlomak& r2) {
    Long r = Razlomak::NZD(r1.nazivnik, r2.nazivnik);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.brojnik, r2.nazivnik/r);
    Razlomak::TestirajPrekoracenjeProizvoda(r2.brojnik, r1.nazivnik/r);
    Long prvi = r1.brojnik * r2.nazivnik / r;
    Long drugi = r2.brojnik * r1.nazivnik / r; 
    Razlomak::TestirajPrekoracenjeSume(prvi, drugi);
    Long rezultantniBrojnik = prvi + drugi;
    Razlomak::TestirajPrekoracenjeProizvoda(r1.nazivnik, r2.nazivnik/r);
    Long rezultantniNazivnik = r1.nazivnik * r2.nazivnik / r;
    return {rezultantniBrojnik, rezultantniNazivnik};
}
Razlomak operator-(const Razlomak& r1, const Razlomak& r2) {
    return r1 + (-r2);
}

void NR() { std::cout << std::endl; }

int main() {
    Razlomak r1{2337,3740}, r2(4014,5225);
    Razlomak r3 = r1 *= r2;
    r1.Ispisi(); NR();
    r2.Ispisi(); NR();
    r3.Ispisi();
    return 0;
}
