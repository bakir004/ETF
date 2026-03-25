#include <iostream>
#include <iomanip>

int main() {
    int m, n;
    std::cin >> m >> n;
    // za npr. n=7, i se krece od -3 do 3
    // prije bi se ubio nego pisao dvije
    // petlje za gornji i donji dio
    //
    for(int i = -n/2; i <= n/2; i++) {
        if(abs(i) == n/2) // izuzeci su prvi i zadnji red
            std::cout << std::setfill('*') << std::setw(m+1) << "\n";
        else {
            // format: razmaci-zvj-razmaci-zvj-razmaci
            for(int j = 0; j < n/2-abs(i); j++) 
                std::cout << " ";

            std::cout << "*";

            for(int j = 0; j < abs(i); j++) 
                std::cout << " ";
            for(int j = 0; j < m-2*(1+n/2); j++) 
                std::cout << " ";
            for(int j = 0; j < abs(i); j++) 
                std::cout << " ";

            std::cout << "*";
            
            for(int j = 0; j < n/2-abs(i); j++) 
                std::cout << " ";

            std::cout << "\n";
        }
    }
    return 0;
}