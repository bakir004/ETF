#include <iostream>
#include <list>

int main() {
    std::list<int> l{1,2,3,4,5,6};
    std::cout << std::distance(l.begin(), l.end());
    return 0;
}
