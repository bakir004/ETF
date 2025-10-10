#include <iostream>
#include <vector>
#include <cmath>

bool HasCompleteSquares(std::vector<int> arr) {
    for(auto num : arr) {
        int root = sqrt(num);
        if(root * root == num)
            return true;
    }
    return false;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for(int i = 0; i < n; i++)
        std::cin >> nums[i];

    std::cout << "Ima potpunih kvadrata: " << std::boolalpha << HasCompleteSquares(nums);
    return 0;
}