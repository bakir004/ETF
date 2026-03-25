#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

typedef vector<vector<int>> Graf;

bool visited[1000001];

bool bfs(Graf& graf, int from, int to) {
    queue<int> red;
    visited[from] = true;
    red.push(from);

    cout << "BFS put: ";
    while(!red.empty()) {
        int trenutni = red.front();
        red.pop();

        cout << trenutni << " ";

        // ovdje radimo sta hocemo sa grafom
        // if(trenutni == to)
        //     return true;

        for(int i = 0 ; i < graf[trenutni].size(); i++) {
            int komsija = graf[trenutni][i];
            if(!visited[komsija]) {
                visited[komsija] = true;
                red.push(komsija);
            }
        }
    }
    return false;
}

bool dfs(Graf& graf, int from, int to) {
    stack<int> red;
    visited[from] = true;
    red.push(from);
    cout << "DFS put: ";

    while(!red.empty()) {
        int trenutni = red.top();
        red.pop();

        cout << trenutni << " ";

        for(int i = 0 ; i < graf[trenutni].size(); i++) {
            int komsija = graf[trenutni][i];
            if(!visited[komsija]) {
                visited[komsija] = true;
                red.push(komsija);
            }
        }
    }
    return false;
}
int main() {
    int n;
    cin >> n;
    //                        koliko cega
    Graf graf(n+1, vector<int>());
    //               aku su cvorovi numerisani od 1 do n

    int m;
    cin >> m;

    for(int i = 0; i < m; i++) {
        int from, to;
        cin >> from >> to;
        graf[from].push_back(to);
        graf[to].push_back(from);
    }

    for(int i = 1; i <= n; i++) {
        cout << i << ": ";
        for(int j = 0; j < graf[i].size(); j++)
            cout << graf[i][j] << ", ";
        cout << "\n";
    }

    bool rezultat = dfs(graf, 1, 6);
    cout << "Postoji put: " << boolalpha << rezultat;
}

