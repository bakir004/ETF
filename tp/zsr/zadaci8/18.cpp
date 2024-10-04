#include <iostream>
#include <vector>
#include <list>

int Poglavica(int N, int M, bool ispis = false) {
    std::list<int> lista;
    auto it = lista.begin();
    for (int i = 1; i <= N; i++) {
        lista.push_back(i);
        if(i <= M)
            it++;
    }
    int zadnji = 0;
    while (!lista.empty()) {
        if(ispis)
            std::cout << *it << ", ";
        zadnji = *it;
        it = lista.erase(it);
        if (it == lista.end())
            it = lista.begin();
        for (int i = 0; i < M - 1; i++) {
            it++;
            if (it == lista.end())
                it = lista.begin();
        }
    }
    if(ispis)
        std::cout << "\n";
    return zadnji;
}

int SigurnoMjesto(int M, int N1, int N2) {
    std::vector<bool> validnaMjesta(N2, true);
    for(int N = N1; N <= N2; N++) {
        int redniBrojPoglavice = Poglavica(N, M, true);
        validnaMjesta[redniBrojPoglavice-1] = false;
    }
    for(int i = 0; i < validnaMjesta.size(); i++)
        if(validnaMjesta[i])
            return i+1;
    return 0;
}

int main() {
    std::cout << SigurnoMjesto(4, 10, 21);
    return 0;
}
