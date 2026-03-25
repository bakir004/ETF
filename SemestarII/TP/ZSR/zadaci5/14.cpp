#include <iostream>

template <typename T>
int BrojParnih(T *pocetak, T *kraj) {
    int brojac = 0;
    while(pocetak < kraj)
        if(*pocetak++ % 2 == 0)
            brojac++;
    return brojac;
}

int main() {
    int niz[10]{1,2,3,4,5,6,7,8,9,10};
    std::cout << BrojParnih(niz, niz + 10) << std::endl;
    char niz2[9] = "abcdefgh";
    std::cout << BrojParnih(niz2, niz2 + 9) << std::endl;
    return 0;
}