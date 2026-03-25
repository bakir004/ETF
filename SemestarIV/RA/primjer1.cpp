#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

class Matrix {
    vector<int> data;
    int size;

public:
    Matrix(int s) : size(s), data(s * s) {
        for (auto& val : data) val = rand() % 100;
    }

    int* operator[](int row) {
        return &data[row * size];
    }

    int getSize() const { return size; }
};

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

// Ovaj primjer pokazuje prostornu lokalnost, gdje pristup
// elementima matrice po redovima bude brze, jer se
// blokovi iz memorije upisuju u kes i time daje
// vise kes hitova, dok po kolonama ima vise promasaja

int main() {
    const int N = 10000;
    Matrix mat(N);
    volatile long long sum = 0;

    {
        Timer t("Row-Major (Sequential)");
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                sum += mat[i][j]; 
            }
        }
    }

    {
        Timer t("Column-Major (Strided)");
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                sum += mat[i][j]; 
            }
        }
    }

    return 0;
}
