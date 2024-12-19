#include <iostream>

int main() {
    std::cout << saberi(2,3);
    std::cout << "N IN RAJVOSA";
    return 0;
}

// 1. g++ -O3 -c -x c++-header -o pch.gch pch.h 
// 2. g++ -O3 -include pch.h -c b.cpp
// 3. g++ -O3 b.o -o program
