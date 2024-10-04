#include <iostream>
#include <stack>

std::stack<int> Funkcija(std::stack<int> s, int x) {
    std::stack<int> rezultat;
    std::stack<int> temp;
    while(!s.empty()) {
        temp.push(s.top() * x);
        s.pop();
    }
    while(!temp.empty()) {
        rezultat.push(temp.top());
        temp.pop();
    }
    return rezultat;
}

int main() {
    std::stack<int> s1;
    s1.push(1); s1.push(2); s1.push(3); s1.push(4);
    std::stack<int> s2 = Funkcija(s1, 2);
    while(!s2.empty()) {
        std::cout << s2.top() << " ";
        s2.pop();
    }
    return 0;
}
