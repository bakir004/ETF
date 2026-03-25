#include <iostream>

using namespace std;

bool visited[1000];
int dubina[1000];

bool bfs(Graf& graf, int from, int to) {
    queue<int> red;
    visited[from] = true;
    red.push(from);
    dubina[from] = 0;

    while(!red.empty()) {
        int trenutni = red.front();
        red.pop();

        if(trenutni == to && dubina[trenutni] < 6)

        for(int i = 0 ; i < graf[trenutni].size(); i++) {
            int komsija = graf[trenutni][i];
            if(!visited[komsija]) {
                visited[komsija] = true;
                dubina[komsija] = dubina[trenutni]+1;
                red.push(komsija);
            }
        }
    }
    return false;
}

int main() {
    int sum = 0;
    for(int i = 0; i < 1e9; i++)
        sum += 1;
    cout << sum;
}

