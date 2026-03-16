#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main() {
    const int N = 100000000;
    vector<float> data(N, 1.1f);
    volatile float sum = 0;

    {
        auto start = chrono::high_resolution_clock::now();
        float s = 0;
        for (int i = 0; i < N; ++i) {
            s += data[i];
        }
        sum = s;
        auto end = chrono::high_resolution_clock::now();
        cout << "Standard Loop: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << " ms\n";
    }

    {
        auto start = chrono::high_resolution_clock::now();
        float s1 = 0, s2 = 0, s3 = 0, s4 = 0;
        for (int i = 0; i < N; i += 4) {
            s1 += data[i];
            s2 += data[i+1];
            s3 += data[i+2];
            s4 += data[i+3];
        }
        sum = s1 + s2 + s3 + s4;
        auto end = chrono::high_resolution_clock::now();
        cout << "Unrolled (4x): " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << " ms\n";
    }

    return 0;
}
