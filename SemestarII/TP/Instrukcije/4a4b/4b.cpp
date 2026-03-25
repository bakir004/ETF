#include<iostream>

// int PomnoziSaN(int x, int n = 2) {
//     return x * n;
// }

// void CrtajPravouagonik(int a, int b) {
//     std::cout << "Funkcija sa 2 parametra\n";
//     for(int i = 0; i < a; i++) {
//         std::cout << std::string(b, '*') << "\n";
//     }
// } 
// void CrtajPravouagonik(int a, char b) {
//     std::cout << "Funkcija sa 2 parametra i char\n";
//     for(int i = 0; i < a; i++) {
//         std::cout << b << "\n";
//     }
// } 
// void CrtajPravouagonik(int a) {
//     std::cout << "Funkcija sa 1 parametrom\n";
//     for(int i = 0; i < a; i++) {
//         std::cout << '*' << "\n";
//     }
// } 

// void IspisiBroj(int x) {
//     std::cout << "Int verzija\n";
//     std::cout << x;
// }
// void IspisiBroj(double x) {
//     std::cout << "Double verzija\n";
//     std::cout.precision(5);
//     std::cout << std::fixed << x;
// }

// void Razmijeni(int& x1, int& x2) {
//     std::cout << "Int verzija\n";
//     std::swap(x1,x2);
// }
// void Razmijeni(double& x1, double& x2) {
//     std::cout << "Double verzija\n";
//     std::swap(x1,x2);
// }

// void Razmijeni(std::string& x1, std::string& x2) {
    // std::swap(x1,x2);
// }

// template <typename NekiTip1, typename NekiTip2>
// void Razmijeni(NekiTip1 x1, NekiTip2 x2) {
//     // std::swap(x1,x2);
// }

template <typename KontejnerskiTip>
void IspisiKolekciju(KontejnerskiTip kontejner) {
    for(int i = 0; i < kontejner.size(); i++) {
        std::cout << kontejner[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> niz{1,6,3,5,9,4};
    std::deque<int> dek{5,9,4,22,33,44};
    std::string s = "hello world";
    IspisiKolekciju(niz);
    IspisiKolekciju(dek);
    IspisiKolekciju(s);
    IspisiKolekciju(std::string("hello world"));
    // std::string a = "hello";
    // std::string b = "world";
    // double a = 3.4;
    // int b = 6.2;
    // Razmijeni(a, b);
    // std::cout << "a: " << a << "\n";
    // std::cout << "b: " << b;
    // IspisiBroj(4);
    // CrtajPravouagonik(4);
    // CrtajPravouagonik(4, '.');
    // int a = 4;
    // a = PomnoziSaN(a);
    // std::cout << a;
    return 0;
}
