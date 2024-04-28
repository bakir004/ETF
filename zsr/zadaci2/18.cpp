#include<iostream>
#include<vector>

std::vector<int> EvenThenOdd(int* arr, int size) {
    std::vector<int> res;
    for(int i = 0; i < size; i++)
        if(arr[i] % 2 == 0)
            res.push_back(arr[i]);
    for(int i = 0; i < size; i++)
        if(arr[i] % 2 == 1)
            res.push_back(arr[i]);
    return res;
}

int main() {
    int arr[10];
    for(int i = 0; i < 10; i++)
        std::cin >> arr[i];
    std::vector<int> res = EvenThenOdd(arr, 10);
    for(int i = 0; i < res.size(); i++)
        std::cout << res[i] << " ";
    return 0;
}