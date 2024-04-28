#include <algorithm>
#include <iostream>
#include <string>
#include <deque>

int main() {
    std::deque<std::string> stringovi(10);
    for(int i = 0; i < 10; i++)
        std::getline(std::cin, stringovi[i]);

    std::copy_if(stringovi.begin(), stringovi.end(), stringovi.begin(), [](std::string s){
            int samoglasnici = 0;
            for(int i = 0; i < s.length(); i++) {
                char c = std::toupper(s[i]);
                if(c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
                    samoglasnici++;
                if(samoglasnici >= 5)
                    return false;
            }
            return true;
        });
    std::cout << "\n";
    for(auto s : stringovi)
        std::cout << s << "\n";

    return 0;
}