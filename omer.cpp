#include <algorithm>
#include <iostream>

class Klasa1 {
public:
    virtual void Ispisi() { std::cout << "nigga1\n"; }
};

class Klasa2 : public Klasa1 {
public:
    void Ispisi() { std::cout << "nigga2\n"; }
};

int main() {
    Klasa1* k1 = new Klasa1();   
    k1->Ispisi();
    Klasa2* k2 = new Klasa2();   
    k2->Ispisi();
    Klasa1* k3 = new Klasa2();   
    k3->Ispisi();
    // int* p1 = new int[5];
    // for(int i = 0; i < 5; i++)
    //     p1[i] = i;
    //
    // int* p2 = new int[5];
    // std::copy(p1, p1+5, p2);
    //
    // // uduplavanje
    // for(int i = 0; i < 5; i++) 
    //     p2[i] *= 2;
    //
    // for(int i = 0; i < 5; i++)
    //     std::cout << p1[i] << " ";
    // std::cout << '\n';
    //
    // for(int i = 0; i < 5; i++)
    //     std::cout << p2[i] << " ";

    // int** matrica = new int*[5]{};
    // 
    // for(int i = 0; i < 5; i++) {
    //     matrica[i] = new int[5];
    //     for(int j = 0; j < 5; j++)
    //         matrica[i][j] = i+j;
    // }
    // for(int i = 0; i < 5; i++) {
    //     for(int j = 0; j < 5; j++) {
    //         std::cout << matrica[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }
    //
    // int** kopija = new int*[5]{};
    // 
    // for(int i = 0; i < 5; i++) {
    //     kopija[i] = new int[5];
    //     for(int j = 0; j < 5; j++) {
    //         kopija[i][j] = matrica[i][j];
    //     }
    // }
    // std::cout << "\n";
    //
    //
    // kopija[3][3]=-99999;
    // for(int i = 0; i < 5; i++) {
    //     for(int j = 0; j < 5; j++) {
    //         std::cout << kopija[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    return 0;
}
