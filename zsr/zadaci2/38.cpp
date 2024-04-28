#include <iostream>
#include <vector>

// postoji bibliotecni find(), ali iteratore nismo radili :(
int Find(std::vector<int> arr, int target) {
    int indexOfFoundElement = -1;
    for(int i = 0; i < arr.size(); i++)
        if(arr[i] == target) indexOfFoundElement = i;
    return indexOfFoundElement;
}

std::vector<int> OdstraniDuplikate(std::vector<int> arr) {
    std::vector<int> unique;
    for(int i = 0; i < arr.size(); i++) 
        if(Find(unique, arr[i]) == -1)
            unique.push_back(arr[i]);
    return unique;
}

int BrojDuplikata(std::vector<int> arr) {
    std::vector<int> unique;
    for(int i = 0; i < arr.size(); i++) 
        if(Find(unique, arr[i]) == -1)
            unique.push_back(arr[i]);
    return arr.size() - unique.size();
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for(int i = 0; i < n; i++)
        std::cin >> nums[i];
    std::vector<int> unique = OdstraniDuplikate(nums);
    int duplicates = BrojDuplikata(nums);
    std::cout << "Duplikata je: " << duplicates << "\n";
    std::cout << "Unikatni su: ";
    for(int i = 0; i < unique.size(); i++)
        std::cout << unique[i] << " ";

    return 0;
}