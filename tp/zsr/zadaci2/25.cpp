#include <iostream>
#include <vector>

// oblik mozemo napisati kao n = (k-2) + (k-1) + k + (k+1) + (k+2)
// nakon kracenja ostane n = 5k, dakle ako je broj djeljiv sa 5, onda
// se moze napisati u obliku navedenom gore
std::vector<bool> MoguKaoZbirPetUzastopnih(std::vector<int> nums) {
    std::vector<bool> possible(nums.size(), false);
    for(int i = 0; i < nums.size(); i++)
        if(nums[i] >= 15 && nums[i] % 5 == 0)
            possible[i] = true;
    return possible;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for(int i = 0; i < n; i++)
        std::cin >> nums[i];
    std::vector<bool> possible = MoguKaoZbirPetUzastopnih(nums);
    for(int i = 0; i < n; i++)
        std::cout << std::boolalpha << possible[i] << " ";
    return 0;
}