#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
T Medijan(std::vector<T> niz) {
    int n = niz.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (niz.at(i) > niz.at(j)) {
                T temp = niz.at(i);
                niz.at(i) = niz.at(j);
                niz.at(j) = temp;
            }
        }
    }
    return niz.at(n / 2);
}

template <typename T>
void IzbaciElemente(std::vector<T> &niz1, std::vector<T> &niz2, bool veci) {
    if(niz1.size() == 0)
        throw std::logic_error("Vektor 1 je prazan");
    if(niz2.size() == 0)
        throw std::logic_error("Vektor 2 je prazan");
    T medijan = Medijan(niz2);
    // std::cout << "Medijan drugog niza: " << medijan << std::endl;
    // std::cout << "Izbacujem " << (veci ? "vece" : "manje") << std::endl;
    for(int i = 0; i < niz1.size(); i++) {
        if((veci && niz1.at(i) > medijan) || (!veci && niz1.at(i) < medijan)) {
            niz1.erase(niz1.begin() + i);
            i--;
        }
    }
}

int main() {
    std::vector<double> niz1{1.2,9.3,2.5,10.25,555};
    std::vector<double> niz2{3.4,3.6,3.1,6.25,560};
    try {
        IzbaciElemente(niz1, niz2, true);
        for(auto &i : niz1)
            std::cout << i << " ";
        std::cout << "\n";
    } catch(std::logic_error err) {
        std::cout << "Greska: " << err.what() << std::endl;
    }

    std::vector<std::string> niz3{"a", "b", "c", "d", "DEEDE"};
    std::vector<std::string> niz4{"bakir", "dzelo", "c", "ajdin", "pilici"};

    try {
        IzbaciElemente(niz3, niz4, false);
        for(auto &i : niz3)
            std::cout << i << " ";
        std::cout << "\n";
    } catch(std::logic_error err) {
        std::cout << "Greska: " << err.what() << std::endl;
    }

    std::vector<double> niz5{};
    std::vector<double> niz6{3.4,3.6,3.1,6.25,560};
    try {
        IzbaciElemente(niz6, niz5, true);
    } catch(std::logic_error err) {
        std::cout << "Greska: " << err.what() << std::endl;
    }

    try {
        IzbaciElemente(niz5, niz6, true);
    } catch(std::logic_error err) {
        std::cout << "Greska: " << err.what() << std::endl;
    }

    return 0;
}























