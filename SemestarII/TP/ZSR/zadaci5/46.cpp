#include <iostream>
#include <deque>
#include <algorithm>
#include <vector>

bool KriterijSortiranja(const int &a, const int &b) {
    int djeliociA = 0, djeliociB = 0;
    for(int i = 1; i <= std::max(a,b); i++) {
        if(a >= i && a % i == 0)
            djeliociA++;
        if(b >= i && b % i == 0)
            djeliociB++;
    }
    if(djeliociA != djeliociB)
        return djeliociA > djeliociB;
    return a < b;
}

template <typename T>
void SortirajPoBrojuDjelilaca(T p1, int len) {
    std::sort(p1, p1 + len, KriterijSortiranja);
}

template <typename T>
bool PretraziBinarno(T p1, int len, int trazeniElement) {
    return std::binary_search(p1, p1 + len, trazeniElement, KriterijSortiranja);
}

int main() {
    std::deque<int> dek{1,2,3,4,5,6,7,8,9,10,12,24};
    std::vector<int> v{1,2,3,4,5,6,7,8,9,10,12,24};
    SortirajPoBrojuDjelilaca(dek.begin(), dek.size());
    SortirajPoBrojuDjelilaca(v.begin(), v.size());
    for(auto c : dek)
        std::cout << c << " ";
    std::cout << "\n";
    for(auto c : dek)
        std::cout << c << " ";
    std::cout << "\n";
    std::cout << PretraziBinarno(dek.begin(), dek.size(), 24) << "\n";
    std::cout << PretraziBinarno(v.begin(), v.size(), 24);
    return 0;
}