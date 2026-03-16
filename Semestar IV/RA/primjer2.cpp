#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

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

void run_experiment(const vector<int>& data, string label) {
    volatile long long sum = 0; // volatile to prevent compiler from skipping the loop
    const int iterations = 100;
    
    Timer t(label);
    for (int it = 0; it < iterations; ++it)
        for (int i = 0; i < data.size(); ++i)
            if (data[i] >= 128)
                sum += data[i];
}

int main() {
    const int SIZE = 32768;
    vector<int> data(SIZE);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    for (int& x : data) x = dis(gen);

    run_experiment(data, "Unsorted (Random)");

    sort(data.begin(), data.end());
    run_experiment(data, "Sorted (Predictable)");

    return 0;
}
