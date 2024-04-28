#include <iostream>
#include <vector>

int FlipDigits(int n) {
    int flipped = 0;
    int flipSign = n < 0 ? -1 : 1;
    n *= flipSign;
    while(n > 0) {
        int digit = n % 10;
        n /= 10;
        flipped = flipped * 10 + digit;
    }
    flipped *= flipSign;
    return flipped;
}

std::vector<int> SlikeUOgledalu(std::vector<int> nums) {
    std::vector<int> reverses;
    for(int num : nums)
        reverses.push_back(FlipDigits(num));
    return reverses;
}

int main() {
    int n;
    std::vector<int> nums;
    while(std::cin >> n, n != 0)
        nums.push_back(n);
    std::vector<int> reverses = SlikeUOgledalu(nums);
    for(int i = 0; i < reverses.size(); i++)
        std::cout << "Slika u ogledalu broja " << nums[i] << " glasi " << reverses[i] << "\n";
    return 0;
}