#include <iostream>
#include <vector>
#include <cmath>

std::vector<int> A(int n) {
    std::vector<int> kvadrati;
    for(int i = 1; i < n; i++) {
        double korijen = sqrt(i);
        if((int)korijen == korijen)
            kvadrati.push_back(i);
    }
    return kvadrati;
}

void B(std::vector<int> &niz) {
    for(auto &e : niz)
        e = (4 * e + 7) % 19; 
}

void C(std::vector<int> &niz, int &min, int &max) {
    min = 9, max = 0;
    for(auto &e : niz) {
        min = std::min(min, e);
        max = std::max(max, e);
    }
}

bool D(std::vector<int> &niz) {
    for(auto &e : niz)
        if(e % 2 == 0) return true;
    return false;
}

template <typename T>
void E(T *pocetak, T *izaZadnjeg) {
    while(pocetak < izaZadnjeg)
        std::cout << *pocetak++ << " ";
}

int main() {
    std::vector<int> kvadrati = A(100);
    for(auto k : kvadrati)
        std::cout << k << " ";
    std::cout << std::endl;
    B(kvadrati);
    for(auto k : kvadrati)
        std::cout << k << " ";
    std::cout << std::endl;
    int minimalni, maksimalni;
    C(kvadrati, minimalni, maksimalni);
    std::cout << "MIN: " << minimalni << ", MAX: " << maksimalni << "\n";
    std::cout << "Ima parnih: " << std::boolalpha << D(kvadrati) << "\n";
    E(&kvadrati[0], &kvadrati[0] + kvadrati.size());
    return 0;
}