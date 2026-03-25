#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main() {
    int n;
    cin >> n;
    unordered_map<string, int> bodoviTakmicara;
    int brojTakmicaraPoBodovima[1001];
    int kumulativni[1001];
    // vector<int> brojTakmicaraPoBodovima(1001, 0)
    for(int i = 0; i < 1001; i++) {
        brojTakmicaraPoBodovima[i] = 0;
        kumulativni[i] = 0;
    }

    for(int i = 0; i < n; i++) {
        string takmicar;
        int bodovi;
        cin >> takmicar >> bodovi;
        bodoviTakmicara[takmicar] = bodovi;
        brojTakmicaraPoBodovima[bodovi]++;
    }

    kumulativni[0] = brojTakmicaraPoBodovima[0];
    for(int i = 1; i < 1001; i++)
        kumulativni[i] = kumulativni[i-1] + brojTakmicaraPoBodovima[i];

    int m;
    cin >> m;
    int brojTakmicara = bodoviTakmicara.size();
    for(int i = 0; i < m; i++) {
        string takmicar;
        cin >> takmicar;
        if(bodoviTakmicara.count(takmicar) == 0) {
            cout << "Nepoznata sifra.\n";
            continue;
        }
        int brojBodovaTrenutnogTakmicara = bodoviTakmicara[takmicar];

        cout << "Osvojili ste " << brojBodovaTrenutnogTakmicara << " bodova. Nalazite se na ";
        int brojBoljih = brojTakmicara - kumulativni[brojBodovaTrenutnogTakmicara];
        cout << brojBoljih + 1 << " mjestu od " << brojTakmicara << " takmicara.\n";
    }
}
