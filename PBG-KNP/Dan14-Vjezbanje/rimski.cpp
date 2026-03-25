#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <cmath>

using namespace std;

int RimskiUDec(string rimski) {
    unordered_map<char, int> rimskiUDec{
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };
    int dec = 0;
    for(int i = 0; i < rimski.size(); i++) {
        int vrijednostTrenutnog = rimskiUDec[rimski[i]];
        int vrijednostSljedeceg = rimskiUDec[rimski[i+1]];
        if(i != rimski.size() - 1 && vrijednostTrenutnog < vrijednostSljedeceg)
            dec -= vrijednostTrenutnog;
        else 
            dec += vrijednostTrenutnog;
    }
    return dec;
}

string DecURimski(int n) {
    map<int, string> decURimski{
        {1, "I"},
        {4, "IV"},
        {5, "V"},
        {9, "IX"},
        {10, "X"},
        {40, "XL"},
        {50, "L"},
        {90, "XC"},
        {100, "C"},
        {400, "CD"},
        {500, "D"},
        {900, "CM"},
        {1000, "M"},
    };
    // int n = 0
    // rimski = "MCCXXXIV"

    string rimski = "";
    int brojac = 10;
    while(n > 0 && brojac-- > 0) {
        int najvecaVrijednostDosad = 0;
        for(pair<int,string> par : decURimski) {
            int vrijednost = par.first;
            if(vrijednost <= n)
                najvecaVrijednostDosad = vrijednost;
            else break;
        }
        rimski += decURimski[najvecaVrijednostDosad];
        n -= najvecaVrijednostDosad;
    }
    return rimski;
}

void solve() {
    string rimski1, rimski2;
    char znak;
    string ulaz;

    cin >> ulaz;
    bool vidjenOperator = false;
    for(auto& c : ulaz) {
        if(c == '+') {
            vidjenOperator = true;
            znak = c;
        } else if(c == '-') {
            vidjenOperator = true;
            znak = c;
        } else if(c == '*') {
            vidjenOperator = true;
            znak = c;
        } else if(c == '/') {
            vidjenOperator = true;
            znak = c;
        } else if(c == '^') {
            vidjenOperator = true;
            znak = c;
        } else if(c == '%') {
            vidjenOperator = true;
            znak = c;
        } else {
            if(!vidjenOperator)
                rimski1 += c;
            else 
                rimski2 += c;
        }
    }
    switch(znak) {
        case '+':
            cout << DecURimski(RimskiUDec(rimski1) + RimskiUDec(rimski2));
            break;
        case '-':
            cout << DecURimski(RimskiUDec(rimski1) - RimskiUDec(rimski2));
            break;
        case '*':
            cout << DecURimski(RimskiUDec(rimski1) * RimskiUDec(rimski2));
            break;
        case '/':
            cout << DecURimski(RimskiUDec(rimski1) / RimskiUDec(rimski2));
            break;
        case '^':
            cout << DecURimski(pow(RimskiUDec(rimski1), RimskiUDec(rimski2)));
            break;
        case '%':
            cout << DecURimski(RimskiUDec(rimski1) % RimskiUDec(rimski2));
            break;
    }
    cout << '\n';
}

int main() {
    int t;
    cin >> t;
    while(t--)
        solve();
}
