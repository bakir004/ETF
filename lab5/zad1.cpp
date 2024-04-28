#include <cctype>
#include <iostream>

int main() {
    char str[1000];
    std::cout << "Unesite recenicu: ";
    scanf("%[^\n]", str);
    char *p = str;
    while(*p++ == ' ');
    while(isalpha(*p++));
    while(*p++ == ' ');
    p--;
    int razlika = p-str;
    while(*(p-razlika) = *p)
        p++;

    std::cout << str;
}

