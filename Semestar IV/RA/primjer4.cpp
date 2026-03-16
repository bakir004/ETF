#include <iostream>
#include <thread>
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

struct Contended {
    volatile long long countA = 0;
    volatile long long countB = 0;
};

struct Padded {
    volatile long long countA = 0;
    long long padding[8];
    volatile long long countB = 0;
};

void work(volatile long long& counter) {
    for (int i = 0; i < 100000000; ++i) counter++;
}

int main() {
    Contended bad;
    {
        Timer t("False Sharing (Contended)");
        thread t1(work, ref(bad.countA));
        thread t2(work, ref(bad.countB));
        t1.join(); t2.join();
    }

    Padded good;
    {
        Timer t("No False Sharing (Padded)");
        thread t1(work, ref(good.countA));
        thread t2(work, ref(good.countB));
        t1.join(); t2.join();
    }

    return 0;
}
