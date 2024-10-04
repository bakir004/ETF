#include <iostream>
#include <deque>
#include <iterator>
#include <type_traits>
#include <vector>
#include <string>

using std::begin;
using std::end;

template <typename IterTip1, typename IterTip2, typename IterTip3>
IterTip3 UnijaBlokova(IterTip1 p1, IterTip1 p2, IterTip2 p3, IterTip2 p4, IterTip3 p5) {
    typedef std::remove_reference_t<decltype(*p1)> T1;
    typedef std::remove_reference_t<decltype(*p3)> T2;
    IterTip3 pocetakBlokaUnije = p5;
    while(p1 != p2) {
        T1 element = *p1;

        bool postoji = false;
        IterTip3 temp = pocetakBlokaUnije;
        while(temp != p5) {
            if(element == *temp) {
                postoji = true;
                break;
            }
            temp++;
        }
        if(!postoji) {
            *p5 = element;
            p5++;
        }
        p1++;
    }   
    while(p3 != p4) {
        T2 element = *p3;

        bool postoji = false;
        IterTip3 temp = pocetakBlokaUnije;
        while(temp != p5) {
            if(element == *temp) {
                postoji = true;
                break;
            }
            temp++;
        }
        if(!postoji) {
            *p5 = element;
            p5++;
        }
        p3++;
    }   
    return p5;
}

int main() {
    double niz[100];
    int n, m;
    std::cout << "Unesite broj elemenata niza (max. 100): ";
    std::cin >> n;
    std::cout << "Unesite elemente niza: ";
    for(int i = 0; i < n; i++)
        std::cin >> niz[i];

    std::cout << "Unesite broj elemenata deka: ";
    std::cin >> m;
    std::deque<int> dek(m);
    std::cout << "Unesite elemente deka: ";
    for(int i = 0; i < m; i++) 
        std::cin >> dek[i];

    std::vector<int> rez(n + m);
    
    auto izaKraja = UnijaBlokova(niz, niz + n, begin(dek), end(dek), begin(rez));
    int vel = izaKraja - begin(rez);
    rez.resize(vel);
    std::cout << "Njihova unija glasi: ";
    for(auto e : rez)
        std::cout << e << " ";

    return 0;
}
