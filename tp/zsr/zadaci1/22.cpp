#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    int m,n;
    // pretpostavka da je n neparno
    std::cin >> m >> n;
    // recimo za n=7, ici ce od -3 do 3
    for(int i = -(n/2); i <= n/2; i++) {
        if(i == 0) 
            std::cout << std::setfill('*') << std::setw(m+1) << "\n";
        else {
            for(int j = 0; j < abs(i); j++) 
                std::cout << " ";
            std::cout << "*";
            for(int j = 0; j < m-2*abs(i)-2; j++)
                std::cout << " ";
            std::cout << "*";
            for(int j = 0; j < abs(i); j++) 
                std::cout << " ";
            std::cout << "\n";
        }
    }
    return 0;
}