#include <initializer_list>
#include <sstream>
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
    static Long NZD(Long p, Long q) noexcept;
    friend std::ostream& operator<<(std::ostream& tok, const Razlomak& r);
    friend std::istream& operator>>(std::istream& tok, Razlomak& r);
    Razlomak(Long brojnik = 0, Long nazivnik = 1); 
    Razlomak(std::initializer_list<Long> list);
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
    Razlomak& operator++();
    Razlomak operator++(int);
    Razlomak& operator--();
    Razlomak operator--(int);
    friend bool operator<(const Razlomak& r1, const Razlomak& r2);
    friend bool operator>(const Razlomak& r1, const Razlomak& r2);
    friend bool operator<=(const Razlomak& r1, const Razlomak& r2);
    friend bool operator>=(const Razlomak& r1, const Razlomak& r2);
    friend bool operator==(const Razlomak& r1, const Razlomak& r2);
    friend bool operator!=(const Razlomak& r1, const Razlomak& r2);
    explicit operator long double() const;
};
Long Razlomak::NZD(Long p, Long q) noexcept {
    if(q == 0) return p;
    return NZD(q, p % q);
}
void Razlomak::Postavi(Long a = 0, Long b = 1) {
    if(b == 0) throw std::logic_error("Nekorektan razlomak");
    Long nzd = NZD(a, b);
    // std::cout << "PRIJE POSTAVLJANJA: " << a << '/' << b << '\n';
    brojnik = a / nzd;
    nazivnik = b / nzd;
    if(nazivnik < 0) {
        brojnik *= -1;
        nazivnik *= -1;
    }
    // std::cout << "POSTAVICU: " << brojnik << '/' << nazivnik << '\n';
}
void Razlomak::Postavi(const Razlomak& drugi) {
    if(drugi.nazivnik == 0) throw std::logic_error("Nekorektan razlomak");
    brojnik = drugi.brojnik;
    nazivnik = drugi.nazivnik;
    if(drugi.nazivnik == 0) std::cout << "PANIKA\n";
}
Razlomak::operator long double() const {
    return (long double)brojnik / nazivnik;
}
std::ostream& operator<<(std::ostream& tok, const Razlomak& r) {
    if(r.nazivnik == 1) std::cout << r.brojnik;
    else std::cout << r.brojnik << '/' << r.nazivnik;
    return tok;
}
std::istream& operator>>(std::istream& tok, Razlomak& r) {
    Long a, b;
    char c;
    tok >> a;
    if(tok.peek() == EOF || std::isspace(tok.peek())) {
        r.brojnik = a;
        r.nazivnik = 1;
    } else if(tok.peek() == '/') {
        tok >> c;
        if(tok.peek() == '\n') {
            tok.setstate(std::ios::failbit);
            return tok;
        }
        tok >> b;
        if(tok.peek() != EOF && !std::isspace(tok.peek())) {
            tok.setstate(std::ios::failbit);
            return tok;
        }
        Long nzd = Razlomak::NZD(a,b);
        r.brojnik = a/nzd;
        r.nazivnik = b/nzd;
    } else tok.setstate(std::ios::failbit);
    return tok;
}
Razlomak::Razlomak(std::initializer_list<Long> list) {
    if(list.size() == 1) Postavi(*list.begin());
    else if(list.size() == 2) Postavi(*list.begin(), *(list.begin()+1)); 
    else throw std::domain_error("Neispravni parametri (inicijalizacijska lista)");
}
Razlomak::Razlomak(Long brojnik, Long nazivnik)  { Postavi(brojnik, nazivnik); }

bool operator<(const Razlomak& r1, const Razlomak& r2) {
    return (long double)r1.brojnik / r1.nazivnik < (long double)r2.brojnik / r2.nazivnik;
}
bool operator>(const Razlomak& r1, const Razlomak& r2) {
    return (long double)r1.brojnik / r1.nazivnik > (long double)r2.brojnik / r2.nazivnik;
}
bool operator<=(const Razlomak& r1, const Razlomak& r2) {
    return (long double)r1.brojnik / r1.nazivnik < (long double)r2.brojnik / r2.nazivnik || r1 == r2;
}
bool operator>=(const Razlomak& r1, const Razlomak& r2) {
    return (long double)r1.brojnik / r1.nazivnik > (long double)r2.brojnik / r2.nazivnik || r1 == r2;
}
bool operator==(const Razlomak& r1, const Razlomak& r2) {
    return r1.brojnik == r2.brojnik && r1.nazivnik == r2.nazivnik;
}
bool operator!=(const Razlomak& r1, const Razlomak& r2) {
    return r1.brojnik != r2.brojnik || r1.nazivnik != r2.nazivnik;
}
Razlomak& Razlomak::operator++() {
    Postavi(*this + 1);
    return *this;
}
Razlomak Razlomak::operator++(int) {
    Razlomak stari = *this;
    Postavi(*this + 1);
    return stari;
}
Razlomak& Razlomak::operator--() {
    Postavi(*this - 1);
    return *this;
}
Razlomak Razlomak::operator--(int) {
    Razlomak stari = *this;
    Postavi(*this - 1);
    return stari;
}
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
    if(LONG_LONG_MAX - std::abs(a) < std::abs(b)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}
void Razlomak::TestirajPrekoracenjeProizvoda(Long a, Long b) {
    if(LONG_LONG_MAX / std::abs(a) < std::abs(b)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}
Razlomak operator*(const Razlomak& r1, const Razlomak& r2) {
    Long s = Razlomak::NZD(r1.brojnik, r2.nazivnik);
    Long t = Razlomak::NZD(r2.brojnik, r1.nazivnik);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.brojnik/s, r2.brojnik/t);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.nazivnik/t, r2.nazivnik/s);
    return {(r1.brojnik/s) * (r2.brojnik/t), (r1.nazivnik/t) * (r2.nazivnik/s)};
}
Razlomak operator/(const Razlomak& r1, const Razlomak& r2) {
    Long r = Razlomak::NZD(r1.nazivnik, r2.nazivnik);
    Long u = Razlomak::NZD(r1.brojnik, r2.brojnik);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.brojnik/u, r2.nazivnik/r);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.nazivnik/r, r2.brojnik/u);
    return {(r1.brojnik/u) * (r2.nazivnik/r), (r1.nazivnik/r) * (r2.brojnik/u)};
}
Razlomak operator+(const Razlomak& r1, const Razlomak& r2) {
    Long r = Razlomak::NZD(r1.nazivnik, r2.nazivnik);
    Razlomak::TestirajPrekoracenjeProizvoda(r1.brojnik, r2.nazivnik/r);
    Razlomak::TestirajPrekoracenjeProizvoda(r2.brojnik, r1.nazivnik/r);
    Long prvi = r1.brojnik * (r2.nazivnik / r);
    Long drugi = r2.brojnik * (r1.nazivnik / r); 
    Razlomak::TestirajPrekoracenjeSume(prvi, drugi);
    Long rezultantniBrojnik = prvi + drugi;
    Razlomak::TestirajPrekoracenjeProizvoda(r1.nazivnik, r2.nazivnik/r);
    Long rezultantniNazivnik = r1.nazivnik * (r2.nazivnik / r);
    return {rezultantniBrojnik, rezultantniNazivnik};
}
Razlomak operator-(const Razlomak& r1, const Razlomak& r2) {
    return r1 + (-r2);
}

int main() {
    Razlomak r1 (12345432LL, 12234432LL);
    r1 *= (r1 + Razlomak (1));
    std::cout << r1 << std::endl;
    r1 /= (r1 + Razlomak (1542LL));
    std::cout << r1 << std::endl;
    r1 = r1 + Razlomak (9939LL, 7);
    std::cout << r1 << std::endl;
    return 0;
}

