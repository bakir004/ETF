#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

class Vector;

class BAZNAVector {
public:
    virtual int NElems() const { std::cout << "niggson nisi implementiro"; return 0; };
    /*virtual double &operator[](int i);*/
    /*virtual double operator[](int i) const;*/
    /*virtual double &operator()(int i);*/
    /*virtual double operator()(int i) const;*/
    /*virtual double Norm() const;*/
    /*virtual double GetEpsilon() const ;*/
    /*virtual void Print(char separator = '\n', double eps = -1) const;*/
    /*virtual Vector &operator +=(const Vector &v);*/
    /*virtual Vector &operator -=(const Vector &v);*/
    /*virtual Vector &operator *=(double s);*/
    /*virtual Vector &operator /=(double s);*/

    friend double VectorNorm(const Vector &v);
    friend void PrintVector(const Vector &v, char separator, double eps);
    friend Vector operator +(const Vector &v1, const Vector &v2);
    friend Vector operator -(const Vector &v1, const Vector &v2);
    friend Vector operator *(double s, const Vector &v);
    friend Vector operator *(const Vector &v, double s);
    friend double operator *(const Vector &v1, const Vector &v2);
    friend Vector operator /(const Vector &v, double s);
};

class Vector : public BAZNAVector {
    std::vector<double> elementi;
public:
    Vector() = default;
    explicit Vector(int n) { 
        if(n <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<double>(n, 0); 
    }
    Vector(std::initializer_list<double> l) {
        if(l.size() <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<double>(l);
    }
    double &operator[](int i) { return elementi[i]; };
    double operator[](int i) const { return elementi[i]; };
    double &operator()(int i) {
        if(i < 1 || i > NElems()) throw std::range_error("Invalid index");
        return elementi[i-1];
    }
    double operator()(int i) const {
        if(i < 1 || i > NElems()) throw std::range_error("Invalid index");
        return elementi[i-1];
    };
    double Norm() const {
        double s = 0;
        std::for_each(elementi.begin(), elementi.end(), [&s](double x){s += x*x;});
        return std::sqrt(s);
    };
    friend double VectorNorm(const Vector &v);
    double GetEpsilon() const {
        return 10 * Norm() * std::numeric_limits<double>::epsilon();
    }
    void Print(char separator = '\n', double eps = -1) const {
        if(eps < 0) eps = GetEpsilon();
        for(int i = 0; i < NElems(); i++) {
            if(std::fabs(elementi[i]) < eps)
                std::cout << 0;
            else std::cout << elementi[i];
            if(i != NElems()-1 || separator == '\n') std::cout << separator;
        }
    }
    friend void PrintVector(const Vector &v, char separator, double eps);
    friend Vector operator +(const Vector &v1, const Vector &v2);
    Vector &operator +=(const Vector &v) {
        if(v.NElems() != NElems()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < NElems(); i++)
            elementi[i] += v[i];
        return *this;
    }
    friend Vector operator -(const Vector &v1, const Vector &v2);
    Vector &operator -=(const Vector &v) {
        if(v.NElems() != NElems()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < NElems(); i++)
            elementi[i] -= v[i];
        return *this;
    }
    friend Vector operator *(double s, const Vector &v);
    friend Vector operator *(const Vector &v, double s);
    Vector &operator *=(double s) {
        for(int i = 0; i < NElems(); i++)
            elementi[i] *= s;
        return *this;
    }
    friend double operator *(const Vector &v1, const Vector &v2);
    friend Vector operator /(const Vector &v, double s);
    Vector &operator /=(double s) {
        if(std::fabs(s) <= std::numeric_limits<double>::epsilon()) throw std::domain_error("Division by zero");
        for(int i = 0; i < NElems(); i++)
            elementi[i] /= s;
        return *this;
    }

    // ZADACA 2
    void Chop(double eps = -1) {
        if(eps < 0) eps = GetEpsilon();
        std::for_each(elementi.begin(), elementi.end(), [&eps](double &x){if(std::fabs(x) < eps) x = 0;});
    }
    bool EqualTo(const Vector& v, double eps = -1) const {
        if(eps < 0) eps = GetEpsilon();
        if(NElems() != v.NElems()) return false;
        for(int i = 0; i < NElems(); i++)
            if(std::fabs(elementi[i] - v[i]) > eps) return false;
        return true;
    }
};

Vector operator +(const Vector &v1, const Vector &v2) {
    if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    Vector rezultat(v1.NElems());
    for(int i = 0; i < rezultat.NElems(); i++)
        rezultat[i] = v1[i] + v2[i];
    return rezultat;
}

Vector operator -(const Vector &v1, const Vector &v2) {
    if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    Vector rezultat(v1.NElems());
    for(int i = 0; i < rezultat.NElems(); i++)
        rezultat[i] = v1[i] - v2[i];
    return rezultat;
}

Vector operator *(double s, const Vector &v) {
    Vector rezultat(v.NElems());
    for(int i = 0; i < v.NElems(); i++)
        rezultat[i] = v[i];
    std::for_each(rezultat.elementi.begin(), rezultat.elementi.end(), [&s](double &x){x*=s;});
    return rezultat;
}

Vector operator *(const Vector &v, double s) { return s * v; }

double operator *(const Vector &v1, const Vector &v2) {
    if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    double s = 0;
    for(int i = 0; i < v1.NElems(); i++)
        s += v1[i] * v2[i];
    return s;
}

Vector operator /(const Vector &v, double s) {
    if(std::fabs(s) <= std::numeric_limits<double>::epsilon()) throw std::domain_error("Division by zero");
    Vector rezultat(v.NElems());
    for(int i = 0; i < v.NElems(); i++)
        rezultat[i] = v[i];
    std::for_each(rezultat.elementi.begin(), rezultat.elementi.end(), [&s](double &x){x/=s;});
    return rezultat;
}

void PrintVector(const Vector &v, char separator = '\n', double eps = -1) {
    if(eps < 0) eps = v.GetEpsilon();
    for(int i = 0; i < v.elementi.size(); i++) {
        if(std::fabs(v[i]) < eps)
            std::cout << 0;
        else std::cout << v[i];
        if(i != v.NElems()-1 || separator == '\n') std::cout << separator;
    }
}

struct OverloadTag {};


/*double VectorNorm(const Vector &v) {*/
/*    double s = 0;*/
/*    std::for_each(v.elementi.begin(), v.elementi.end(), [&s](double x){s += x*x;});*/
/*    return std::sqrt(s);*/
/*}*/
template <typename T = OverloadTag>
double VectorNorm(Vector v, T = {}) {
    std::cout << "?";
    return 0;
}

int main() {
    Vector x({1, 2, 3, 4, 5});
    std::cout << x.NElems() << '\n';
    std::cout << '\n' << VectorNorm(x);
}
