#include <iostream>

class LinFun {
  double k,l;
public:
  LinFun(double k, double l) : k(k), l(l) {}
  LinFun(double l = 0) : k(0), l(l) {}
  LinFun& Postavi(double k, double l) {
    this->k = k;
    this->l = l;
    return *this;
  }
  double DajK() const { return k; }
  double DajL() const { return l; }
  LinFun& operator-() {
    this->k = -this->k;
    this->l = -this->l;
    return *this;
  }
  LinFun& operator+=(const LinFun& x) {
    *this = *this + x;
    return *this;
  }
  LinFun& operator-=(const LinFun& x) {
    *this = *this - x;
    return *this;
  }
  LinFun& operator*=(double x) {
    *this = *this * x;
    return *this;
  }
  LinFun& operator/=(double x) {
    *this = *this / x;
    return *this;
  }
  LinFun& operator++() { this->l++; return *this; }
  LinFun operator++(int) { this->l++; return LinFun(k, --l); }
  double operator()(double x) const { return k * x + l;}
  LinFun Inverz() const { 
    if(k == 0) throw;
    return LinFun(1/k, -l/k);
  }
  friend LinFun operator+(const LinFun& f1, const LinFun& f2);
  friend LinFun operator-(const LinFun& f1, const LinFun& f2);
  friend LinFun operator*(const LinFun& f, double x);
  friend LinFun operator/(const LinFun& f, double x);
};

LinFun operator+(const LinFun& f1, const LinFun& f2) {
  return LinFun(f1.DajK() + f2.DajK(), f1.DajL() + f2.DajL());
}
LinFun operator-(const LinFun& f1, const LinFun& f2) {
  return LinFun(f1.DajK() - f2.DajK(), f1.DajL() - f2.DajL());
}
LinFun operator*(const LinFun& f, double x) {
  return LinFun(f.DajK() * x, f.DajL() * x);
}
LinFun operator*(double x, const LinFun& f) {
  return f * x;
}
LinFun operator/(const LinFun& f, double x) {
  return f * 1/x;
}
LinFun operator/(double x, const LinFun& f) {
  return f / x;
}

int main() {
  LinFun f(3,5);
  LinFun fp = f.Inverz();
  std::cout << f(fp(3));
  return 0;
}
