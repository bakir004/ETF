#include <iostream>
#include <vector>
#include <cmath>

auto Polinom(std::vector<int> koef) {
    return [koef](int x){
        int rezultat = 0;
        for(int i = 0; i < koef.size(); i++) {
            rezultat += koef[i] * pow(x, i);
        }
        return rezultat;
    };
}

int main() {
    auto poli = Polinom({2,3,0,-1,4});
    std::cout << poli(5);
    return 0;
}
