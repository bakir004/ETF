#include<iostream>
#include<cmath>
#include<stdexcept>
#include<vector>

std::vector<bool> CanBeExpressedAsCube(std::vector<int> arr) {
    std::vector<bool> possibleCubes;
    for(int num : arr) {
        int root = cbrt(num);
        if(num < 0) {
            throw std::domain_error("Negativan element!");
        }
        possibleCubes.push_back(root * root * root == num);
    }
    return possibleCubes;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for(int i = 0; i < n; i++)
        std::cin >> nums[i];

    try {
        std::vector<bool> possibleCubes = CanBeExpressedAsCube(nums);
        for(int i = 0; i < possibleCubes.size(); i++)
            std::cout << possibleCubes[i] << " ";
    } catch (std::domain_error err) {
        std::cout << err.what();
    }
    return 0;
}