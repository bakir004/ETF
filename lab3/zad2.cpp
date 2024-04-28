#include <iostream>
#include <vector>

int ZbirCifara(int broj)
{
    broj = abs(broj);
    int zbir = 0;
    while (broj > 0)
    {
        zbir += (broj % 10);
        broj /= 10;
    }
    return zbir;
}

std::vector<int> IzdvojiElemente(std::vector<int> niz, bool parno)
{
    std::vector<int> novi;
    for (auto element : niz)
        if (ZbirCifara(element) % 2 == (parno ? 0 : 1))
            novi.push_back(element);
    return novi;
}

int main()
{
    int n;
    std::cout << "Koliko zelite unijeti elemenata: ";
    std::cin >> n;
    std::cout << "Unesite elemente: ";
    std::vector<int> niz;
    for (int i = 0; i < n; i++)
    {
        int element;
        std::cin >> element;
        niz.push_back(element);
    }
    std::vector<int> parni = IzdvojiElemente(niz, true);
    std::vector<int> neparni = IzdvojiElemente(niz, false);

    for (int i = 0; i < parni.size(); i++) {
        std::cout << parni[i];
        if(i != parni.size()-1)
            std::cout << ",";
    }
    std::cout << "\n";
    for (int i = 0; i < neparni.size(); i++) {
        std::cout << neparni[i];
        if(i != neparni.size()-1)
            std::cout << ",";
    }
    std::cout << "\n";

    return 0;
}
