#include <iostream>
#include <algorithm>
#include <deque>

int main() {
    std::deque<int> brojevi{12,24,360,1,2,3,4,5,6,7};
    std::sort(brojevi.begin(), brojevi.end(), [](const int &a, const int &b) {
                int djeliociA = 0, djeliociB = 0;
                for(int i = 1; i <= std::max(a,b); i++) {
                    if(a >= i && a % i == 0)
                        djeliociA++;
                    if(b >= i && b % i == 0)
                        djeliociB++;
                }
                if(djeliociA != djeliociB)
                    return djeliociA > djeliociB;
                return a < b;
            });
    for(auto broj : brojevi)
        std::cout << broj << " ";
    return 0;
}