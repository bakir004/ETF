#include <iostream>

using namespace std;

int main() {
    int sum = 0;
    for(int i = 0; i < 1e9; i++)
        sum += 1;
    cout << sum;
}
