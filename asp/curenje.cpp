#include <iostream>

void leak() {
    int* a = new int;
}

int main() {
    leak();
    return 0;
}
