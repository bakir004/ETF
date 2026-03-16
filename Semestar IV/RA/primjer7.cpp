#include <iostream>
#include <vector>
#include <chrono>
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
    const int N = 100000000;
    vector<int> dataA(N, 1);
    vector<int> dataB(N, 0);
    
    volatile long long junk_sum = 0; 

    {
        Timer t("Scenario A: Simple Copy       ");
        for (int i = 0; i < N; ++i) {
            dataB[i] = dataA[i];
        }
    }

    fill(dataB.begin(), dataB.end(), 0);

    {
        Timer t("Scenario B: Copy + 'Free' Math");
        for (int i = 0; i < N; ++i) {
            dataB[i] = dataA[i];
            
            junk_sum += (i * 3) + 10; 
        }
    }

    cout << "Final junk_sum: " << junk_sum << endl;

    return 0;
}
