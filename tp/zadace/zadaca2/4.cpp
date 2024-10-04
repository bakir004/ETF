#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
bool Kriterij(const std::vector<T> &v1, const std::vector<T> &v2) {
    T max1 = *std::max_element(v1.begin(), v1.end());
    T max2 = *std::max_element(v2.begin(), v2.end());
    if(max1 != max2)
        return max1 > max2;
    return v1 > v2;
}

template <typename T>
auto SortirajPoDobrotiRedova(std::vector<std::vector<T>> &matrica) -> std::vector<std::vector<T>> {
    std::sort(matrica.begin(), matrica.end(), Kriterij<T>);
    return matrica;
}

int main() {
    std::vector<std::vector<int>> matrica;

    std::cout << "Unesite elemente (* za kraj reda, * na pocetku reda za kraj unosa):\n";
    int n;
    std::vector<int> trenutni;
    while(true) {
        while(std::cin >> n)
            trenutni.push_back(n);
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        if(trenutni.empty())
            break;
        matrica.push_back(trenutni);
        trenutni.clear();
    }
    SortirajPoDobrotiRedova(matrica);

    std::cout << "\nMatrica nakon sortiranja:\n";
    for(auto v : matrica) {
        for(auto e : v)
            std::cout << e << " ";
        std::cout << std::endl;
    }

    std::cout << "\nUnesite elemente sekvence koja se trazi (* za kraj reda): ";
    while(std::cin >> n)
        trenutni.push_back(n);

    auto iterator = std::lower_bound(matrica.begin(), matrica.end(), trenutni, Kriterij<int>);

    if(iterator != matrica.end() && *iterator == trenutni)
        std::cout << "Trazena sekvenca se nalazi u " << iterator - matrica.begin() + 1 << ". redu (nakon sortiranja)";
    else 
        std::cout << "Trazena sekvenca se ne nalazi u matrici";

    return 0;
}
