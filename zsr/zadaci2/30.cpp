#include <iostream>
#include <vector>

bool IsPerfect(int n) {
    int sum = 0;
    for(int i = 1; i < n; i++)
        if(n % i == 0)
            sum += i;
    return sum == n;
}

std::vector<int> FilterPerfectNums(std::vector<int> nums) {
    std::vector<int> perfectNums;
    for(int i = 0; i < nums.size(); i++)
        if(IsPerfect(nums[i]))
            perfectNums.push_back(nums[i]);
    return perfectNums;
}

int main() {
    int n;
    std::vector<int> nums;
    while(std::cin >> n, n != 0)
        nums.push_back(n);
    std::vector<int> perfectNums = FilterPerfectNums(nums);
    for(int i = 0; i < perfectNums.size(); i++)
        std::cout << perfectNums[i] << " ";
    return 0;
}