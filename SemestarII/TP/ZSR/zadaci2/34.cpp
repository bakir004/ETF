#include <iostream>
#include <vector>

int Factorial(int n) {
    if(n < 0) return 0;
    int product = 1;
    for(int i = 2; i <= n; i++)
        product *= i;
    return product;
}

std::vector<int> MapToFactorial(std::vector<int> arr) {
    std::vector<int> factorials(arr.size());
    for(int i = 0; i < arr.size(); i++)
        factorials[i] = Factorial(arr[i]);
    return factorials;
}

int main() {
    int n;
    std::vector<int> nums;
    while(std::cin >> n, n != 0)
        nums.push_back(n);
    std::vector<int> factorials = MapToFactorial(nums);
    for(int i = 0; i < factorials.size(); i++)
        std::cout << factorials[i] << " ";
    return 0;
}