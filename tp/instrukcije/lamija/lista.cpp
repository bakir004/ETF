#include<iostream>

template <typename T>
struct Cvor {
    T vrijednost;
    Cvor* sljedeci;
};

int main() {

    Cvor<int>* pocetak = new Cvor<int>{0,nullptr};
    Cvor<int>* prosli = pocetak;

    for(int i = 1; i <= 5; i++) {
        Cvor<int>* novi = new Cvor<int>{i,nullptr};
        prosli->sljedeci = novi; 
        prosli = prosli->sljedeci;
    }


    for(Cvor<int>* pomocni = pocetak; pomocni != nullptr; pomocni = pomocni->sljedeci) {
        std::cout << pomocni->vrijednost << " ";
    }
    
    Cvor<std::string>* pocetak2 = new Cvor<std::string>{"strin",nullptr};
    Cvor<std::string>* prosli2 = pocetak2;

    for(int i = 1; i <= 5; i++) {
        Cvor<std::string>* novi2 = new Cvor<std::string>{"string",nullptr};
        prosli2->sljedeci = novi2; 
        prosli2 = prosli2->sljedeci;
    }


    for(Cvor<std::string>* pomocni2 = pocetak2; pomocni2 != nullptr; pomocni2 = pomocni2->sljedeci) {
        std::cout << pomocni2->vrijednost << " ";
    }
    return 0;
}
