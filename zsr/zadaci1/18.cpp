#include <iomanip>
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++)
            std::cout << "+-----";
        std::cout << "+\n|";
        for(int j = 1; j <= n; j++) {
            std::cout << std::setw(4) << i*j << " |";
        }
        std::cout << "\n";
    }
    for(int i = 1; i <= n; i++)
        std::cout << "+-----";
    std::cout << "+";
    return 0;
}