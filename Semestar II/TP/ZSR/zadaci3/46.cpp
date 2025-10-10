#include<iostream>
#include<string>
#include<vector>

std::string DecToBin(int n) {
    std::string res, temp;
    if(n < 0) {
        n = -n;
        res += '-';
    }
    while(n > 0) {
        temp += char('0' + n % 2);
        n /= 2;
    }
    for(int i = temp.length() - 1; i >= 0; i--)
        res += temp[i];
    return res;
}

int main() {
    int n;
    std::cin >> n;
    std::cout << DecToBin(n);
    return 0;
}