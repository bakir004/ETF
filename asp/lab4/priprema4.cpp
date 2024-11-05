#include<iostream>
#include <vector>

using namespace std;
template <typename T>
class Red {
    struct Cvor {
        Cvor* sljedeci;
        Cvor* prethodni;
        T element;
    };
    Cvor *back, *front;
    int velicina;
public:
    Red(): back(nullptr), front(nullptr), velicina(0) {}
    Red(const Red& r): back(nullptr), front(nullptr), velicina(0) {
        Cvor* temp = r.front;
        while(temp != nullptr) {
            stavi(temp->element);
            temp = temp->prethodni;
        }
    }
    Red& operator=(const Red& r) {
        if(this == &r) return *this;
        brisi();
        Cvor* temp = r.front;
        while(temp != nullptr) {
            stavi(temp->element);
            temp = temp->prethodni;
        }
        return *this;
    }
    void stavi(const T& el) {
        Cvor* novi = new Cvor{nullptr, nullptr, el};
        if(back == nullptr)
            front = novi;
        else back->prethodni = novi;
        novi->sljedeci = back;
        back = novi;

        velicina++;
    }
    T skini() {
        if(velicina == 0)
            throw std::domain_error("Red je prazan");
        T el = front->element;
        Cvor* temp = front;
        front = front->prethodni;
        if(front == nullptr)
            back = nullptr;
        else front->sljedeci = nullptr;
        delete temp;
        velicina--;
        return el;
    }
    void brisi() {
        while(back != nullptr) {
            Cvor* temp = back;
            back = back->sljedeci;
            delete temp;
        }
        velicina = 0;
    }
    void ispisi() {
        Cvor* temp = back;
        while(temp != nullptr) {
            std::cout << temp->element << " ";
            temp = temp->sljedeci;
        }
        std::cout << std::endl;
    }
    T& celo() { return front->element; }
    int brojElemenata() { return velicina; }
    ~Red() { brisi(); }
};

int main() {
}
