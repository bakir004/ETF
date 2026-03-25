#include <iostream>
#include <vector>

enum class Kriterij {Veca, Manja, Jednaka};

int DigitProduct(int n) {
    int product = 1;
    while(n > 0) {
        int digit = n%10;
        n /= 10;
        product *= digit;
    }
    return product;
}

int MultOtpornost(int n) {
    int counter = 0;
    if(n < 0) n *= -1;
    while(n / 10 > 0) {
        n = DigitProduct(n);
        counter++;
    }
    return counter;
}

std::vector<int> IBSZMU(std::vector<int> nums, int resistance, Kriterij criteria) {
    std::vector<int> res;
    for(int i = 0; i < nums.size(); i++) {
        if(criteria == Kriterij::Jednaka && MultOtpornost(nums[i]) == resistance)
            res.push_back(nums[i]);
        if(criteria == Kriterij::Manja && MultOtpornost(nums[i]) < resistance)
            res.push_back(nums[i]);
        if(criteria == Kriterij::Veca && MultOtpornost(nums[i]) > resistance)
            res.push_back(nums[i]);
    }
    return res;
}

int main() {
    int n, resistance;
    std::vector<int> nums;
    while(std::cin >> n, n != 0)
        nums.push_back(n);
    std::cin >> resistance;
    std::vector<int> equal = IBSZMU(nums, resistance, Kriterij::Jednaka);
    std::vector<int> less = IBSZMU(nums, resistance, Kriterij::Manja);
    std::vector<int> greater = IBSZMU(nums, resistance, Kriterij::Veca);

    std::cout << "\nJednaki: ";
    for(int i = 0; i < equal.size(); i++)
        std::cout << equal[i] << " ";
    std::cout << "\nManji: ";
    for(int i = 0; i < less.size(); i++)
        std::cout << less[i] << " ";
    std::cout << "\nVeci: ";
    for(int i = 0; i < greater.size(); i++)
        std::cout << greater[i] << " ";
    return 0;
}