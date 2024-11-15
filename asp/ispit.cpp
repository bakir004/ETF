#include <iostream>

int main() {
    int n, s;
    for(int i = 0; i < n*n*n; i++)
        for(int j = 0; j < n*n; j++)
            if(i > n && j < n)
                for(int k = 0; k < n*n*n*n; k++)
                    s++;
    return 0;
}
