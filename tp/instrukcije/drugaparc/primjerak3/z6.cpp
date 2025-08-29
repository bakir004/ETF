#include<iostream>
#include<algorithm>
#include<functional>
#include<stdexcept>

class Vektor : public std::vector<double> {
public:
  Vektor() : std::vector<double>() {}
  Vektor(int x, int y = 0) : std::vector<double>(x,y) {}
  Vektor(std::initializer_list<double> l) : std::vector<double>(l) {}
  void Sortiraj();
  void Sortiraj(std::function<bool(double,double)> f);
  bool DaLiSadrzi(double) const;
  bool operator!() const;
  Vektor& operator+=(const Vektor&);
  double& operator()(int index);
  explicit operator double() const {
    double suma = 0;
    for(auto e : *this)
      suma += e;
    return suma;
  }
  Vektor& operator++();
  Vektor operator++(int);
};

std::ostream& operator<<(std::ostream& tok, const Vektor& v) {
  tok << "[";
  for(int i = 0; i < v.size(); i++) {
    if(i != v.size()-1) tok << v[i] << ",";
    else tok<< v[i];
  }
  tok << "]";
  return tok;
}
Vektor& Vektor::operator++() {
  for(int i = 0; i < this->size(); i++) {
    (*this)[i]++;
  }
  return *this;
}
Vektor Vektor::operator++(int) {
  Vektor kopija = *this;
  for(int i = 0; i < this->size(); i++) {
    (*this)[i]++;
  }
  return kopija;
}

void Vektor::Sortiraj() {
  for(int i = 0; i < this->size(); i++)
    for(int j = i+1; j < this->size(); j++)
      if((*this)[i] > (*this)[j])
        std::swap((*this)[i], (*this)[j]);
}
void Vektor::Sortiraj(std::function<bool(double,double)> f) {
  for(int i = 0; i < this->size(); i++)
    for(int j = i+1; j < this->size(); j++)
      if(!f((*this)[i], (*this)[j]))
        std::swap((*this)[i], (*this)[j]);
}
bool Vektor::DaLiSadrzi(double x) const {
  return std::find(this->begin(), this->end(), x) != this->end();
}
bool Vektor::operator!() const {
  return this->empty();
}
double& Vektor::operator()(int index) {
  if(index < 1 || index > this->size()) 
    throw std::out_of_range("Neispravan index");
  return (*this)[index-1];
}
Vektor& Vektor::operator+=(const Vektor& v) {
  for(int i = 0; i < v.size(); i++) {
    this->push_back(v[i]);
  }
  return *this;
}
Vektor operator+(const Vektor& v1, const Vektor& v2) {
  Vektor v3; 
  for(int i = 0; i < v1.size(); i++)
    v3.push_back(v1[i]);
  for(int i = 0; i < v2.size(); i++)
    v3.push_back(v2[i]);
  return v3;
}
Vektor operator*(const Vektor& v1, const Vektor& v2) {
  if(v1.size() != v2.size()) throw std::logic_error("Nesaglasne dimenzije");
  Vektor v3;
  for(int i = 0; i < v1.size(); i++)
    v3.push_back(v1[i]*v2[i]);
  return v3;
}

int main() {
  Vektor v{1,22,33,4,5};
  Vektor v2{44,55,66,1,2};
  // v.Sortiraj([](double x, double y){ return x < y; });
  Vektor v3 = v * v2;
  std::cout << ++v3 << "\n";
  std::cout << v3 << "\n";


  return 0;
}
