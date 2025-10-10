#include<iostream>
#include<string>
#include<vector>


int main() {
    int m, n;
    char c1, c2;
    std::cin >> n >> m >> c1 >> c2;

    for(int i = m; i >= m-2*(n-1); i-=2) {
        if(i == m) std::cout << std::string(i, c1);
        else if(i == m-2*(n-1)) std::cout << std::string((m-i)/2, ' ') << std::string(m-2*(n-1), c1);
        else {
            std::cout << std::string((m - i)/2, ' ');
            std::cout << c1;
            std::cout << std::string(i - 2, c2);
            std::cout << c1;
            std::cout << std::string((m - i)/2, ' ');
        }
        std::cout << "\n";
    }

    return 0;
}