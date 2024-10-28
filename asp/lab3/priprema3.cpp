#include<iostream>
#include<vector>

// write me a function that takes 2 matrices as vector of vectors and sums them up and write the implementation
std::vector<std::vector<int>> sumMatrices(std::vector<std::vector<int>> a, std::vector<std::vector<int>> b) {
    int n = a.size();
    std::vector<std::vector<int>> c(n, std::vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}


// function taht prints the matrix
void printMatrix(std::vector<std::vector<int>> a) {
    int n = a.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// write a class stack<T> that implements a single linked list with functions, push, top, pop, empty, size
template<typename T>
class stack {
    struct Node {
        T data;
        Node* next;
        Node(T data, Node* next) : data(data), next(next) {}
    };
    Node* head;
    int n;
public:
    stack() : head(nullptr), n(0) {}
    void push(T data) {
        head = new Node(data, head);
        n++;
    }
    T top() {
        return head->data;
    }
    void pop() {
        Node* old = head;
        head = head->next;
        delete old;
        n--;
    }
    bool empty() {
        return n == 0;
    }
    int size() {
        return n;
    }
    // write destructor
    ~stack() {
        while(head != nullptr) {
            Node* old = head;
            head = head->next;
            delete old;
        }
    }
};


int main() {
    // use the stack and fill it with numbers from 1 to 10
    // and test all the methods inside
    stack<int> s;
    for(int i = 1; i <= 10; i++) {
        s.push(i);
    }
    while(!s.empty()) {
        std::cout << s.top() << " ";
        s.pop();
    }
    std::cout << std::endl;
    
    return 0;
}
