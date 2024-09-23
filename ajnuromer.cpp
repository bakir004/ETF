#include <iostream>
#include <stdexcept>

template <typename T>
class Queue {
    struct Cvor {
        T element;
        Cvor* sljedeci;
    };

    Cvor* front;
    Cvor* back;
    int duzina;

public:
    Queue(): front(nullptr), back(nullptr), duzina(0) {}
    Queue(const Queue& drugi): front(nullptr), back(nullptr), duzina(0) {
        if(drugi.duzina == 0) return;
        try {
            for(int pomjeraj = drugi.duzina-1; pomjeraj >= 0; pomjeraj--) {
                Cvor* trenutni = drugi.back;
                for(int i = 0; i < pomjeraj; i++)
                    trenutni = trenutni->sljedeci;
                Push(trenutni->element);
            }
        } catch(...) {
            Cvor* trenutni = back;
            while(trenutni != nullptr) {
                trenutni = trenutni->sljedeci;
                delete back;
                back = trenutni;
            }
        }
    }
    Queue& operator=(const Queue& drugi) {
        Cvor* trenutni = back;
        while(trenutni != nullptr) {
            trenutni = trenutni->sljedeci;
            delete back;
            back = trenutni;
        }
        back = front = nullptr;
        if(drugi.duzina == 0)  {
            duzina = 0;
            return *this;
        }
        try {
            for(int pomjeraj = drugi.duzina-1; pomjeraj >= 0; pomjeraj--) {
                Cvor* trenutni = drugi.back;
                for(int i = 0; i < pomjeraj; i++)
                    trenutni = trenutni->sljedeci;
                Push(trenutni->element);
            }
        } catch(...) {
            Cvor* trenutni = back;
            while(trenutni != nullptr) {
                trenutni = trenutni->sljedeci;
                delete back;
                back = trenutni;
            }
        }
        return *this;
    }

    void Push(T element) {
        if(duzina == 0) {
            Cvor* novi = new Cvor{element, nullptr};
            front = novi;
            back = novi;
        } else {
            Cvor* novi = new Cvor{element, back};
            back = novi;
        }
        duzina++;
    }

    void Pop() {
        if(duzina <= 0) throw std::domain_error("HALO");
        Cvor* trenutni = back;
        while(trenutni->sljedeci != front)
            trenutni = trenutni->sljedeci;

        trenutni->sljedeci = nullptr;
        delete front;
        front = trenutni;
        duzina--;
    }
    void Ispis() const {
        Cvor* trenutni = back; 
        while(trenutni != nullptr) {
            std::cout << trenutni->element << " ";
            trenutni = trenutni->sljedeci;
        }
        std::cout << "\n";
    }

    T& Front() const { return front->element; }
    T Front() { return front->element; }
    T& Back() const { return back->element; }
    T Back() { return back->element; }
    int Duzina() const { return duzina; }

    ~Queue() {
        Cvor* trenutni = back;
        while(trenutni != nullptr) {
            trenutni = trenutni->sljedeci;
            delete back;
            back = trenutni;
        }
    }
};

int main() {
    Queue<int> q;
    q.Push(1);
    q.Push(2);
    q.Push(3);
    q.Push(4);
    q.Push(5);
    q.Pop();
    q.Ispis();
    std::cout << "KOPIRAM\n";
    Queue<int> q2(q);
    q2.Pop();
    q2.Pop();
    q2.Ispis();
    Queue<int> q3;
    q3.Push(69);
    q3.Push(420);
    q3 = q2;
    q3.Ispis();
    return 0;
}
