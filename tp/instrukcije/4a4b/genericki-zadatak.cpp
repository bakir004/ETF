#include<iostream>

template <typename NekiTip>
void ObrisiElement(std::vector<NekiTip>& v, int index) {
    for(int i = index; i < v.size()-1; i++) {
        v[i] = v[i+1];
    }
    v.pop_back();
}

template <typename NekiTip>
void UkloniProsjecne(std::vector<NekiTip>& v) {
    double suma = 0;
    for(int i = 0; i < v.size(); i++) {
        suma += v[i];
    }
    double prosjek = suma / v.size();

    for(int i = 0; i < v.size(); i++) {
        if(v[i] > prosjek) {
            ObrisiElement(v, i);
            i--;
        }
    }
}

int main() {
    std::vector<double> v{1.1,2.1,8.1,9.1,1.1};
    // ObrisiElement(v, 2);
    // ObrisiElement(v, 2);
    UkloniProsjecne(v);
    for(int i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    return 0;
}
