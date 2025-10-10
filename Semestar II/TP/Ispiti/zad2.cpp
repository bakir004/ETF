#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

int main() {
    std::vector<std::list<int>> vektorListi{{1,2,3},{7,8,9,10},{4,5,6}};

    std::sort(vektorListi.begin(), vektorListi.end(), [](std::list<int> l1, std::list<int> l2) {
            int l1Sum = 0, l2Sum = 0;
            auto p1 = l1.begin(), p2 = l2.begin();
            while(p1 != l1.end())
                l1Sum += *p1++;
            while(p2 != l2.end())
                l2Sum += *p2++;
            return l1Sum < l2Sum;
            });
    for(auto l : vektorListi) {
        for(auto e : l)
            std::cout << e << " ";
        std::cout << "\n";
    }
    return 0;
}
