#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <cmath>

using namespace std;

void radixSort(std::vector<int> &a) {
    std::array<std::vector<int>, 10> cifre;
    bool postojeCifre = false;
    int stepenCifre = 0;
    while(true) {
        postojeCifre = false;
        for(int i = 0; i < a.size(); i++) {
            int cifra = (a[i] % (int)std::pow(10,stepenCifre+1)) / std::pow(10,stepenCifre);
            if(cifra != 0) postojeCifre = true;
            cifre[cifra].push_back(a[i]);
        }
        std::vector<int> temp;
        for(int i = 0; i < cifre.size(); i++) {
            for(int j = 0; j < cifre[i].size(); j++) {
                temp.push_back(cifre[i][j]);
            }
            cifre[i].clear();
        }
        a = temp;
        stepenCifre++;
        if(!postojeCifre) break;
    }
}

int lijevoDijete(int i) { return 2*i+1; }
int desnoDijete(int i) { return 2*i+2; }

bool jeLiList(int i, std::vector<int> &v) {
    return lijevoDijete(i) >= v.size();
}

void popraviDolje(std::vector<int> &a, int i, int velicina) {
    while (!jeLiList(i, a)) {
        int veci = lijevoDijete(i);
        int dd = desnoDijete(i);

        if (dd < velicina && a[dd] < a[veci])
            veci = dd;

        if (a[i] <= a[veci]) return;

        std::swap(a[i], a[veci]);
        i = veci;
    }
}

void stvoriGomilu(std::vector<int> &a) {
    int velicina = a.size();
    for(int i = velicina/2-1; i >= 0; i--)
        popraviDolje(a, i, velicina);
}

void umetniUGomilu(std::vector<int> &a, int umetnuti, int &velicina) {

}

int izbaciPrvi(std::vector<int> &a, int& velicina) {
    if(velicina==0)
        throw "\nGomila je prazna!\n";
    velicina--;
    std::swap(a[0], a[velicina]);
    if (velicina!=0)
        popraviDolje(a, 0, velicina);
    return a[velicina];
}

void gomilaSort(std::vector<int> &a) {
    int velicina = a.size();
    stvoriGomilu(a);
    for (int i = velicina - 1; i > 0; i--) {
        std::swap(a[0], a[i]);
        popraviDolje(a, 0, velicina);
    }
}

int main() {
    std::vector<int> v{1,6,4,463,235,57,223,43,2};
    gomilaSort(v);
    for(int i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    return 0;
}
