#include <iostream>
#include <vector>
#include <chrono>

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

struct StudentAoS {
    float score;
    char padding[124]; // Simulate name, address, bio, etc.
};

struct StudentSoA {
    vector<float> scores;
    vector<vector<char>> metadata;
    StudentSoA(size_t n) : scores(n, 0.0f) {}
};

// Structure of Arrays vs. Array of Structures
// Racunanje prosjeka studenata se brze radi ako
// su ocjene u posebnom nizu radi prostorne lokalnosti
// Kada su u strukturi, onda u kesu zauzimaju
// vecinu prostora nebitni podaci

int main() {
    const size_t N = 50000000;

    vector<StudentAoS> aos_list(N);
    StudentSoA soa_list(N);

    volatile float sink = 0;

    {
        Timer t("AoS (Fragmented Memory)");
        float sum = 0;
        for (size_t i = 0; i < N; ++i) {
            sum += aos_list[i].score;
        }
        sink = sum;
    }

    {
        Timer t("SoA (Contiguous Memory)");
        float sum = 0;
        for (size_t i = 0; i < N; ++i) {
            sum += soa_list.scores[i];
        }
        sink = sum;
    }

    return 0;
}
