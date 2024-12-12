#include<iostream>

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
    /*void ispisi() {*/
    /*    Cvor* temp = back;*/
    /*    while(temp != nullptr) {*/
    /*        std::cout << temp->element << " ";*/
    /*        temp = temp->sljedeci;*/
    /*    }*/
    /*    std::cout << std::endl;*/
    /*}*/
    T& celo() { return front->element; }
    int brojElemenata() { return velicina; }
    ~Red() { brisi(); }
};

/*template <typename T>*/
/*int dajMax(T r) {*/
/*    std::cout << "Nije implementirano" << std::endl;*/
/*    return 0;*/
/*}*/

/*template <typename T>*/
/*T dajMax(Red<T>& r) {*/
/*    if(r.brojElemenata() == 0)*/
/*        throw std::domain_error("Red je prazan");*/
/*    T max = r.celo();*/
/*    Red<T> temp(r);*/
/*    while(temp.brojElemenata() != 0) {*/
/*        T el = temp.skini();*/
/*        if(el > max)*/
/*            max = el;*/
/*    }*/
/*    return max;*/
/*}*/

int main() {
    try {
        Red<int> r;
        r.stavi(5);
        r.stavi(3);
        r.stavi(7);
        r.stavi(1);
        r.skini();
        std::cout << "Poziv funkcije ispisi, ali ona nije implementirana" << std::endl;
        std::cout << r.celo() << std::endl;
        std::cout << r.brojElemenata() << std::endl;
        r.brisi();
        std::cout << r.brojElemenata() << std::endl;
    } catch(std::exception& e) {
        std::cout << "IZUZETAK: " << e.what() << std::endl;
    }


    return 0;
}
