#include <iostream>
#include <csignal>
#include <cstdlib>
#include <setjmp.h>

jmp_buf env;

void signalHandler(int signal) {
    if (signal == SIGSEGV) {
        std::cerr << "Segmentation fault detected, recovering..." << std::endl;
        longjmp(env, 1);  // Jump back to a safe point
    } else {
        std::cerr << "Other fatal signal detected, terminating." << std::endl;
        std::exit(signal);  // Exit for other fatal signals
    }
}

void outOfBounds() {
    int a[3];
    a[3] = 0;  
}
void leak() {
    int* a = new int[3];
}
void leak2() {
    int* a = new int[3];
}
void nullPointer() {
    int* a = nullptr;
    *a = 0;
}

int testCounter = 0;

int main() {
    if (setjmp(env) != 0) {
        std::cout << "Program continues after error." << std::endl;
        if (testCounter == 1) {
            std::cout << "Test 1 failed." << std::endl;
            goto test2;
        } else if (testCounter == 2) {
            std::cout << "Test 2 failed." << std::endl;
            goto test3;
        } else if (testCounter == 3) {
            std::cout << "Test 3 failed." << std::endl;
            goto test4;
        } else if (testCounter == 4) {
            std::cout << "Test 4 failed." << std::endl;
        }
        return 0;
    }

    std::signal(SIGSEGV, signalHandler);  // Catch segmentation fault

test1:
    std::cout << "TEST1:\n\n";
    testCounter = 1;
    outOfBounds();  
    std::cout << "Program continues after error handling." << std::endl;
test2:
    std::cout << "TEST2:\n\n";
    testCounter = 2;
    leak();  
    std::cout << "Program continues after error handling." << std::endl;
test3:
    std::cout << "TEST3:\n\n";
    testCounter = 3;
    nullPointer();
    std::cout << "Program continues after error handling." << std::endl;
test4:
    std::cout << "TEST4:\n\n";
    testCounter = 4;
    leak2();
    std::cout << "Program continues after error handling." << std::endl;
    return 0;
}
