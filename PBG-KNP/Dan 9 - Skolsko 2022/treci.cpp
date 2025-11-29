#include <iostream>

using namespace std;

void solve() {
    int n;
    cin >> n;
    if(n >= 19) {
        cout << "NO\n";
        return;
    }
    cout << "YES\n";
    int element = 1, counter = 0;
    while(true) {
        cout << element << " ";
        element *= 3;
        counter++;
        if(counter == n) {
            break;
        }
    }
    cout << "\n";
}

int main() {

    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        solve();
    }

    return 0;
}
