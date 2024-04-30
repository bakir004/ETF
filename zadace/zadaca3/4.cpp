#include <list>
#include <iostream>
#include <vector>

std::vector<int> Razbrajanje(int N, int M) {
    std::vector<int> redoslijed;
    std::list<int> lista;
    for(int i = 1; i <= N; i++) lista.push_back(i);
    auto it = lista.begin();
    while(!lista.empty()) {
        redoslijed.push_back(*it);
        lista.erase(it);
        for(int i = 0; i < M; i++) {
            it++;
            if(it == lista.end())
                it = lista.begin();
        }
    }
    return redoslijed;
}

int OdabirKoraka(int N, int K) {
    for(int i = 1; i <= N; i++) {
        std::vector<int> redoslijed = Razbrajanje(N, i);
        if(redoslijed[N-1] == K)
            return i;
    }
    return 0;
}

int main() {
    int N = 10, K = 2;
    int M = OdabirKoraka(N, K);
    std::cout << "M: " << M << "\n";
    for(auto e : Razbrajanje(N, M))
        std::cout << e << " ";
    return 0;
}
