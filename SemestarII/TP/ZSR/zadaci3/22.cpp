#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> Matrix;

std::array<int, 4> RasponMatrice(Matrix matrix) {
    int imin = INT_MAX, imax = -1, jmin = INT_MAX, jmax = -1;

    int size = matrix[0].size();
    if(size == 0)
        throw std::domain_error("Raspon nije definiran za nul-matricu ili praznu matricu");
    for(int i = 1; i < matrix.size(); i++)
        if(matrix[i].size() != size)
            throw std::domain_error("Nekorektna matrica");

    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            if(matrix[i][j] != 0) {
                imin = std::min(imin, i);
                imax = std::max(imax, i);
                jmin = std::min(jmin, j);
                jmax = std::max(jmax, j);
            }
        }
    }

    if(imax == -1)
        throw std::domain_error("Raspon nije definiran za nul-matricu ili praznu matricu");
    return std::array<int, 4>{imin, jmin, imax, jmax};
}

int main() {
    int m, n;
    std::cin >> m >> n;
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n));
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            std::cin >> matrix[i][j];
    try {
        std::array<int, 4> range = RasponMatrice(matrix);
        std::cout << "Raspon je od (" << range[0] + 1 << "," << range[1] + 1 << ") do (" << range[2] + 1 << "," << range[3] + 1 << ")";
    } catch(std::domain_error err) {
        std::cout << err.what();
    }

    return 0;
}