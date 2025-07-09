#include <iostream>
#include <chrono>
#include <random>
#include <cstring>

void multiply_ijk(const double* a, const double* b, double* c, int n) {
    std::memset(c, 0, n * n * sizeof(double));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                c[i * n + j] += a[i * n + k] * b[k * n + j];
}

void multiply_ikj(const double* a, const double* b, double* c, int n) {
    std::memset(c, 0, n * n * sizeof(double));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < n; ++j)
                c[i * n + j] += a[i * n + k] * b[k * n + j];
}

void fill_random(double* m, int n) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dist(0.0, 1.0);
    for (int i = 0; i < n * n; ++i)
        m[i] = dist(gen);
}

int main() {
    int n = 1024; // Adjust size depending on your CPU/RAM
    double* a = new double[n * n];
    double* b = new double[n * n];
    double* c = new double[n * n];

    fill_random(a, n);
    fill_random(b, n);

    auto start = std::chrono::high_resolution_clock::now();
    multiply_ijk(a, b, c, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_ijk = end - start;
    std::cout << "multiply_ijk took " << duration_ijk.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    multiply_ikj(a, b, c, n);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_ikj = end - start;
    std::cout << "multiply_ikj took " << duration_ikj.count() << " seconds\n";

    delete[] a;
    delete[] b;
    delete[] c;
    return 0;
}
