#include<iostream>
#include<vector>

int EvaluateExpression(std::vector<double> arr) {
    int n = arr.size();
    double past = arr[n-1];
    for(int i = n-2; i >= 0; i--) {
        if(past == 0)
            throw "Greska: desilo se dijeljenje s nulom!";
        double curr = arr[i] + 1./past;
        past = curr;
    }
    return past;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<double> arr(n);
    for(int i = 0; i < n; i++)
        std::cin >> arr[i];

    try {
        int expression = EvaluateExpression(arr);
        std::cout << expression;
    } catch(const char message[]) {
        std::cout << message;
    }
    return 0;
}