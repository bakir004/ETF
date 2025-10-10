#include<iostream>
#include<string>

void CrtajTrougao(int n, char c) {
    for(int i = n-1; i >= 0; i--, c++) {
        std::cout << std::string(i, ' ');
        if(i == n-1)
            std::cout << c;
        else if(i == 0)
            std::cout << std::string(2*n-1, c);
        else {
            std::cout << c;
            std::cout << std::string(2*(n-i)-3, ' ');
            // za prethodnu liniju: trust me bro
            std::cout << c;
        }
        std::cout << "\n";
    }
}

int main() {
    int n;
    char c;
    std::cin >> n >> c;
    CrtajTrougao(n, c);
    return 0;
}