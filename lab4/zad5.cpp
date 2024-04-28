#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

template <typename T>

std::vector<T> Presjek(const std::vector<T> &v1, const std::vector<T> &v2)
{
    std::vector<T> presjek;

    for (const T &element : v1)
    {
        if (std::find(v2.begin(), v2.end(), element) != v2.end() &&
            std::find(presjek.begin(), presjek.end(), element) == presjek.end())
        {
            presjek.push_back(element);
        }
    }
    return presjek;
}

int main()
{
    int a, b;
    std::vector<double> res1;
    std::cout << "Test za realne brojeve...\n";
    std::cout << "Unesite broj elemenata prvog vektora: ";
    std::cin >> a;
    std::cout << "Unesite elemente prvog vektora: ";
    std::vector<double> arr1(a, 0);
    for (int i = 0; i < a; i++)
        std::cin >> arr1[i];

    std::cout << "Unesite broj elemenata drugog vektora: ";
    std::cin >> b;
    std::cout << "Unesite elemente drugog vektora: ";
    std::vector<double> arr2(b, 0);
    for (int i = 0; i < b; i++)
        std::cin >> arr2[i];

    res1 = Presjek(arr1, arr2);

    std::cout << "Zajednicki elementi su: ";
    for (int i = 0; i < res1.size(); i++)
        std::cout << res1[i] << " ";
    std::cout << "\n\n";

    char ch;
    int c, d;
    std::vector<std::string> res2;
    std::cout << "Test za stringove...\n";
    std::cout << "Unesite broj elemenata prvog vektora: ";
    std::cin >> c;
    std::cout << "Unesite elemente prvog vektora: ";
    std::vector<std::string> arr3;
    ch = std::getchar();
    for (int i = 0; i < c; i++)
    {
        std::string str;
        std::getline(std::cin, str);
        arr3.push_back(str);
    }

    std::cout << "Unesite broj elemenata drugog vektora: ";
    std::cin >> d;
    std::cout << "Unesite elemente drugog vektora: ";
    std::vector<std::string> arr4;
    ch = std::getchar();
    for (int i = 0; i < d; i++)
    {
        std::string str;
        std::getline(std::cin, str);
        arr4.push_back(str);
    }

    res2 = Presjek(arr3, arr4);

    std::cout << "Zajednicki elementi su: ";
    for (int i = 0; i < res2.size(); i++)
        std::cout << res2[i] << " ";
    std::cout << "\n\n";

    return 0;
}
