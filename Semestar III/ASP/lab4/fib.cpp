#include<iostream>
#include<vector>

int main() {
    int n = 10;
    std::vector<int> v(n+1, 0);
    v[0]=0;
    v[1]=1;
    v[2]=1;
    for(int i = 3; i <= n; i++) {
        v[i] = v[i-1] + v[i-2];
    }
    std::cout << v[n];

    return 0;
}
