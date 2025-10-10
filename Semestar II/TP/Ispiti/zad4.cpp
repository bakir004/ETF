#include <iostream>
#include <deque>
#include <vector>
#include <iterator>

template <typename IterTip1, typename IterTip2, typename Func>
int PopuniPoKriteriju(IterTip1 p1, IterTip1 p2, IterTip2 p3, IterTip2 &p4, Func f) {
    int suma = 0;
    while(p1 != p2) {
        suma += *p1;
        *p3++ = f(*p1);
        p1++;
    }
    p4 = p3;
    return suma;
}

int main() {
    std::vector<int> v;
    int n;
    while(std::cin >> n)
        v.push_back(n);
    std::deque<int> dek(v.size());
    std::deque<int>::reverse_iterator izaKrajaDeka;
    int suma = PopuniPoKriteriju(v.begin(), v.end(), dek.rbegin(), izaKrajaDeka, [](int x){return x*x;});
    std::cout << "Suma: " << suma << "\n";
    dek.resize(std::distance(dek.rbegin(), izaKrajaDeka));
    for(auto i : dek)
        std::cout << i << " ";

    
    return 0;
}
