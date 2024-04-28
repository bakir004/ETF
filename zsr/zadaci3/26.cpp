#include<iostream>
#include<vector>

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> VectorVectorInt;

VectorVectorInt RastuciPodnizovi(VectorInt arr) {
    VectorVectorInt res;
    VectorInt curr;
    for(int i = 0; i < arr.size()-1; i++) {
        if(arr[i] <= arr[i+1]) {
            curr.push_back(arr[i]);
        } else if(curr.size() != 0) {
            curr.push_back(arr[i]);
            res.push_back(curr);
            curr.clear();
        }
    }
    // off-by-one
    if(curr.size() != 0) {
        curr.push_back(arr[arr.size()-1]);
        res.push_back(curr);
        curr.clear();
    }
    return res;
}

VectorVectorInt OpadajuciPodnizovi(VectorInt arr) {
    VectorVectorInt res;
    VectorInt curr;
    for(int i = 0; i < arr.size()-1; i++) {
        if(arr[i] >= arr[i+1]) {
            curr.push_back(arr[i]);
        } else if(curr.size() != 0) {
            curr.push_back(arr[i]);
            res.push_back(curr);
            curr.clear();
        }
    }
    // off-by-one
    if(curr.size() != 0) {
        curr.push_back(arr[arr.size()-1]);
        res.push_back(curr);
        curr.clear();
    }
    return res;
}

int main() {
    VectorInt arr{1, 3, 2, 6, 7, 9, 10, 4, 3, 2, 3};
    auto rastuci = RastuciPodnizovi(arr);
    std::cout << "Rastuci: \n";
    for(auto v : rastuci) {
        for(auto n : v)
            std::cout << n << " ";
        std::cout << "\n";
    }
    auto opadajuci = OpadajuciPodnizovi(arr);
    std::cout << "Opadajuci: \n";
    for(auto v : opadajuci) {
        for(auto n : v)
            std::cout << n << " ";
        std::cout << "\n";
    }
    return 0;
}