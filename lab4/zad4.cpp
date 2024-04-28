#include <iostream>
#include <cctype>

int StrToNum(std::string strNum) {

}

void UnosBroja(std::string prompt, std::string error, int &num) {
    while(true) {
        std::cout << prompt;
        std::string unos;
        std::cin >> unos;
        for(int i = 0; i < unos.length(); i++) {
            if(!(isdigit(unos[i]) || unos[i] == '.')) {
                std::cout << error;
                continue;
            }
        }
        break;
    }

}

int main() {
    
}
