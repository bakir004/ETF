#include <iostream>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <setjmp.h>

jmp_buf env;

void signalHandler(int signal) {
    if (signal == SIGSEGV) {
        std::cerr << "Segmentation fault detected, recovering..." << std::endl;
        longjmp(env, 1); 
    } else if (signal == SIGABRT) {
        std::cerr << "Abort signal detected (likely due to invalid memory access), recovering..." << std::endl;
        longjmp(env, 1);  
    } else {
        std::cerr << "Other fatal signal detected, terminating." << std::endl;
        std::exit(signal);  
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

void doubleFree() {
    int* a = new int[3];
    delete[] a;
    delete[] a;
}
void useAfterFree() {
    int* a = new int[3];
    delete[] a;
    a[0] = 1;
}
void stackOverflow() {
    int a[5];
    a[10] = 1;
}
void heapOverflow() {
    int* a = new int[5];
    a[5] = 1;
    delete[] a;
}
void memoryLeak() {
    int* a = new int[100];
    a[0] = 42;
}
void invalidFree() {
    int a = 10;
    int* ptr = &a;
    delete ptr;
}
void misalignedAccess() {
    char* a = new char[3];
    int* misaligned = reinterpret_cast<int*>(a);
    *misaligned = 42;
    delete[] a;
}
void danglingPointer() {
    int* a = new int(10);
    int* b = a;
    delete a;
    *b = 5;
}

void wildPointer() {
    int* a;
    *a = 1;
}
void nullAfterFree() {
    int* a = new int(10);
    delete a;
    a = nullptr;
    *a = 10;
}

void uninitializedUse() {
    int a;
    int b = a + 1;
    std::cout << b << "NE CRASHA\n";
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
            goto test5;
        } else if (testCounter == 5) {
            std::cout << "Test 5 failed." << std::endl;
            goto test6;
        } else if (testCounter == 6) {
            std::cout << "Test 6 failed." << std::endl;
            goto test7;
        } else if (testCounter == 7) {
            std::cout << "Test 7 failed." << std::endl;
            goto test8;
        } else if (testCounter == 8) {
            std::cout << "Test 8 failed." << std::endl;
            goto test9;
        } else if (testCounter == 9) {
            std::cout << "Test 9 failed." << std::endl;
            goto test10;
        } else if (testCounter == 10) {
            std::cout << "Test 10 failed." << std::endl;
            goto test11;
        } else if (testCounter == 11) {
            std::cout << "Test 11 failed." << std::endl;
            goto test12;
        } else if (testCounter == 12) {
            std::cout << "Test 12 failed." << std::endl;
            goto test13;
        } else if (testCounter == 13) {
            std::cout << "Test 13 failed." << std::endl;
            goto test14;
        } else if (testCounter == 14) {
            std::cout << "Test 14 failed." << std::endl;
            goto test15;
        }
        return 0;
    }

    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGABRT, signalHandler);
    std::signal(SIGFPE, signalHandler);

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
test5:
    std::cout << "TEST5:\n\n";
    testCounter = 5;
    doubleFree();
    std::cout << "Program continues after error handling." << std::endl;
test6:
    std::cout << "TEST6:\n\n";
    testCounter = 6;
    useAfterFree();
    std::cout << "Program continues after error handling." << std::endl;
test7:
    std::cout << "TEST7:\n\n";
    testCounter = 7;
    stackOverflow();
    std::cout << "Program continues after error handling." << std::endl;
test8:
    std::cout << "TEST8:\n\n";
    testCounter = 8;
    heapOverflow();
    std::cout << "Program continues after error handling." << std::endl;
test9:
    std::cout << "TEST9:\n\n";
    testCounter = 9;
    memoryLeak();
    std::cout << "Program continues after error handling." << std::endl;
test10:
    std::cout << "TEST10:\n\n";
    testCounter = 10;
    invalidFree();
    std::cout << "Program continues after error handling." << std::endl;
test11:
    std::cout << "TEST11:\n\n";
    testCounter = 11;
    misalignedAccess();
    std::cout << "Program continues after error handling." << std::endl;
test12:
    std::cout << "TEST12:\n\n";
    testCounter = 12;
    wildPointer();
    std::cout << "Program continues after error handling." << std::endl;
test13:
    std::cout << "TEST13:\n\n";
    testCounter = 13;
    danglingPointer();
    std::cout << "Program continues after error handling." << std::endl;
test14:
    std::cout << "TEST14:\n\n";
    testCounter = 14;
    nullAfterFree();
    std::cout << "Program continues after error handling.AAAAAAAAAAAAA" << std::endl;
test15:
    std::cout << "TEST15:\n\n";
    testCounter = 15;
    uninitializedUse();
    std::cout << "Program continues after error handling." << std::endl;
    std::cout << "CURENJA:\n";
    return 0;
}

// export ASAN_OPTIONS=detect_leaks=1,print_stacktrace=1,halt_on_error=0
// g++ -fsanitize=address -fsanitize-recover=address -g fatalfields.cpp -o fatalfields
