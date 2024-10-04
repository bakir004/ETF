#include <iostream>
#include <stdexcept>

using namespace std;

int Faktorijel(int x) {
    int p = 1;
    if(x < 0) throw std::domain_error("Ne smije negativno");
    for(int i = 2; i <= x; i++) {
        p *= i;
    }
    return p;
}

int main() {
    int n;
    cout << "Unesite broj brojeva: ";
    cin >> n;
    int suma = 0;
    for(int i = 0; i < n; i++) {
        int broj;
        cin >> broj;
        if(broj < 100 || broj > 999) {
            cout << "Mora biti trocifren\n";
            break;
        }

        int cifra = int(broj / 10) % 10;
        suma += Faktorijel(cifra);
    }
    cout << "Suma je: " << suma;
    return 0;
}
