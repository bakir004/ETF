#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

int StrUInt(std::string s) {
    int x = 0;
    for(int i = 0; i < s.length(); i++)
        x += s[i]-'0';
    return x;
}

auto JedinstveniBroj(std::string x) {
    return [x](std::string y){
        return [x, y](std::string z){
            int zbir = StrUInt(x) + StrUInt(y) + StrUInt(z);
            return zbir % 9 == 0 ? 9 : zbir % 9;
        };
    };
}
int main() {
    int n;
    std::cout << "Unesite broj datuma: ";
    std::cin >> n;
    std::cin.ignore(10000, '\n');
    typedef std::unique_ptr<std::string> StrPok;
    std::shared_ptr<StrPok> datumi;
    try { 
        datumi = std::shared_ptr<StrPok>(new StrPok[n], [](StrPok *p){ delete[] p; });
        for(int i = 0; i < n; i++) {
            datumi.get()[i] = std::make_unique<std::string>();
            std::getline(std::cin, *datumi.get()[i]);
        }
        std::sort(datumi.get(), datumi.get() + n, [](const StrPok &p1, const StrPok &p2) {
                std::string dan1 = (*p1).substr(0, 2);
                std::string mjesec1 = (*p1).substr(3, 2);
                std::string godina1 = (*p1).substr(6, 4);
                
                std::string dan2 = (*p2).substr(0, 2);
                std::string mjesec2 = (*p2).substr(3, 2);
                std::string godina2 = (*p2).substr(6, 4);
                return JedinstveniBroj(dan1)(mjesec1)(godina1) < JedinstveniBroj(dan2)(mjesec2)(godina2);
                });

        std::cout << "Sortirani datumi:\n";
        for(int i = 0; i < n; i++)
            std::cout << *datumi.get()[i] << std::endl;
    } catch(...) {
        std::cout << "Nema memorije!";
    }
    return 0;
}
