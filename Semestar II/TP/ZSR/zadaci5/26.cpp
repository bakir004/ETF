#include <deque>
#include <set>
#include <iostream>
#include <string>
#include <vector>

using std::begin;
using std::end;

template <typename IterTip1, typename IterTip2, typename IterTip3>
IterTip3 ZajednickiElementiBlokova(IterTip1 p1, IterTip1 p2, IterTip2 p3, IterTip2 p4, IterTip3 p5) {
    IterTip1 i = p1;
    IterTip3 p6 = p5;
    while(i < p2) {
        IterTip2 j = p3;
        while(j < p4) {
            if(*i == *j) {
                IterTip3 k = p5;
                bool postoji = false;
                while(k < p6) {
                    if(*k == *i) {
                        postoji = true;
                        break;
                    }
                    k++;
                }
                if(!postoji) {
                    *p6 = *i;
                    p6++;
                }
            }
            j++;
        }
        i++;
    }
    return p6;
}

int main() {
    std::deque<int> dek{5, 2, 7, 4, 6, 1, 3, 2, 7, 4};
    int niz[100]{2, 9, 0, 6, 0, 4, 8, 3, 2, 5};
    std::vector<int> rezultat(10);
    auto pokNaKraj = ZajednickiElementiBlokova(begin(dek), end(dek), begin(niz), end(niz), begin(rezultat));
    rezultat.resize(pokNaKraj - begin(rezultat));
    for(auto c : rezultat)
        std::cout << c << " ";
    std::cout << "\n";

    std::vector<int> mojSet{1, 3, 2, 7, 4};
    std::vector<int> v{2, 9, 4, 6, 0, 4, 8, 3, 2, 5};
    std::vector<int> rez(5);
    pokNaKraj = ZajednickiElementiBlokova(begin(mojSet), end(mojSet), begin(v), end(v), begin(rez));
    rez.resize(pokNaKraj - begin(rez));
    for(auto c : rez)
        std::cout << c << " ";
    return 0;
}
