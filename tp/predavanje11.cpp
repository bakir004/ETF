#include <algorithm>
#include <initializer_list>
#include <iostream>

class VektorNd {
    int dimenzija;
    double* koordinate;
public:
    int DajDimenzija() const { return dimenzija; }

    explicit VektorNd(int dimenzija) : dimenzija(dimenzija), koordinate(new double[dimenzija]{}) {
        std::cout << "Konstruktor 1arg\n";
    }
    explicit VektorNd(std::initializer_list<double> lista): dimenzija(lista.size()), koordinate(new double[dimenzija]) { 
        std::cout << "Konstruktor lista\n";
        Postavi(lista); 
    };
    VektorNd(const VektorNd &v);
    VektorNd(VektorNd &&v);
    VektorNd &operator=(const VektorNd &v);
    VektorNd &operator=(VektorNd&& v);
    void Postavi(std::initializer_list<double> lista);
    void Ispisi() const;
    double *begin() const { return koordinate; }
    double *end() const { return koordinate + dimenzija; }
    ~VektorNd() { delete[] koordinate; }
};

void VektorNd::Postavi(std::initializer_list<double> lista) {
    if(lista.size() != dimenzija) 
        std::cout << "Nesaglasne dimenzije!\n";
    else
        std::copy(lista.begin(), lista.end(), koordinate);
}

void VektorNd::Ispisi() const {
    std::cout << "{";
    for(int i = 0; i < dimenzija; i++)
        std::cout << koordinate[i] << (i != dimenzija - 1 ? "," : "");
    std::cout << "}\n";
} 
VektorNd& VektorNd::operator=(const VektorNd &v) {
    double *noviProstor = new double[v.dimenzija];
    std::copy(v.koordinate, v.koordinate + dimenzija, noviProstor);
    delete[] koordinate;
    dimenzija = v.dimenzija; koordinate = noviProstor;
    return *this;
}
VektorNd& VektorNd::operator=(VektorNd&& v) {
    std::cout << "R vrijednosna dodjela\n";
    return *this;
}

VektorNd::VektorNd(const VektorNd &v): dimenzija(v.dimenzija), koordinate(new double[dimenzija]) {
    std::cout << "Kopiranje postojeceg\n";
    std::copy(v.begin(), v.end(), koordinate);
}
VektorNd::VektorNd(VektorNd &&v): dimenzija(v.dimenzija), koordinate(v.koordinate) {
    std::cout << "Pomjeranje r-vrijednosti\n";
    v.koordinate = nullptr;
}

int main() {
    VektorNd v1{1,2,3,4,5};
    VektorNd v2 = v1;
    VektorNd v3{2,3,4,5,6};
    v1 = v3;
    v1.Ispisi();
    v3.Ispisi();
    return 0;
}
