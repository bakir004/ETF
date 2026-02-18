#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;



int main() {
    vector<pair<string,string>> routes = {
        {"DSM", "ORD"},
        {"ORD", "BGI"},
        {"BGI", "LGA"},
        {"SIN", "CDG"},
        {"CDG", "SIN"},
        {"CDG", "BUD"},
        {"DEL", "DOH"},
        {"DEL", "CDG"},
        {"EWR", "HND"},
        {"HND", "ICN"},
        {"HND", "JFK"},
        {"ICN", "JFK"},
        {"JFK", "LGA"},
        {"EYW", "LHR"},
        {"LHR", "SFO"},
        {"SFO", "SAN"},
        {"SFO", "DSM"},
        {"SAN", "EYW"},
    };
    map<string, vector<string>> graf;

    for(int i = 0; i < routes.size(); i++) {
        pair<string, string> trenutnaVeza = routes[i];
        graf[trenutnaVeza.first].push_back(trenutnaVeza.second);
    }
    for(auto par : graf) {
        cout << par.first << ": ";
        for(auto str : par.second)
            cout << str << ", ";
        cout << '\n';
    }
}

