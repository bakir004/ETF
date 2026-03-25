#include <iostream>
#include <vector>
#include <deque>

template <typename IterTip>
double SumaPoKriteriju(IterTip p1, IterTip p2, bool f(decltype(*p1 + *p1))) {
    double sum = 0;
    while(p1 < p2) {
        if(f(*p1))
            sum += *p1;
        p1++;
    }
    return sum;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> niz(n);
    for(int i = 0; i < n; i++)
        std::cin >> niz[i];
    std::cout << SumaPoKriteriju(niz.begin(), niz.end(), [](int c){ return c > 0; }) << "\n";
    std::deque<double> dek(n); // probavamo sa dekom dublova
    for(int i = 0; i < n; i++) {
        dek[i] = niz[i] - 0.5;
        std::cout << dek[i] << " ";
    }
    std::cout << "\n" << SumaPoKriteriju(dek.begin(), dek.end(), [](double c){ return c < 0; }) << "\n";

    return 0;
}