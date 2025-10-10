#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

std::vector<bool> Funkcija(std::vector<int> v) {
    std::vector<bool> mozeKaoKub(v.size(), false);
    for(int i = 0; i < v.size(); i++) {
        int e = v[i];
        if(e < 0) throw std::domain_error("Negativan element!");
        int korijen = std::cbrt(e);
        if(korijen * korijen * korijen == e)
            mozeKaoKub[i] = true;
    }
    return mozeKaoKub;
}

int main() {
    std::vector<int> v{1,2,3,-4,5,6,7,8,9,0};
    try {
        std::vector<bool> mozeKub = Funkcija(v);
        for(auto e : mozeKub)
            std::cout << e << " ";
    } catch(std::domain_error err) {
        std::cout << err.what() << "\n";
    }
    return 0;
}
