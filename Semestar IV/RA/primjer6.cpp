#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>

using namespace std;

struct Timer {
    string name;
    chrono::high_resolution_clock::time_point start;
    Timer(string n) : name(n), start(chrono::high_resolution_clock::now()) {}
    ~Timer() {
        auto end = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << name << ": " << ms << " ms" << endl;
    }
};

int main() {
    const int N = 50000000;
    vector<double> data(N, 1.0);
    volatile double sink = 0;

    {
        Timer t("Long Dependency Only              ");
        double val = 10.0;
        for (int i = 0; i < N; ++i) {
            val = sqrt(val + data[i]); 
        }
        sink = val;
    }

    {
        Timer t("Long Dependency + Independent Work");
        double val = 10.0;
        long long extra_stuff = 0;
        for (int i = 0; i < N; ++i) {
            val = sqrt(val + data[i]);
            extra_stuff += (i % 2); 
        }
        sink = val + (double)extra_stuff;
    }

    return 0;
}
