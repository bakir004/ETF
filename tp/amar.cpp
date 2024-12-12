#include <iostream>
#include <stack>
#include <queue>

void premjestiNeparne(std::queue<std::stack<int>> &s, std::queue<int> r){
    int sumR=0;
    int velicinaR = r.size();
    for(int i=0;i<velicinaR;i++){
        int x = r.front();
        sumR+=x;
    }
    
    int velicinaR2=s.size();
    
    for(int i=0;i<velicinaR2;i++){
        int sumaSteka=0;
        int velicinaSkinutogSteka=s.front().size();
        std::stack<int> kopijaFronta;
        for(int i=0;i<velicinaSkinutogSteka;i++){
            int skinutiSaSteka = s.front().top();
            s.front().pop();
            sumaSteka+=skinutiSaSteka;
            kopijaFronta.push(skinutiSaSteka);
            r.push(skinutiSaSteka);
        }
        s.pop();
        if(sumaSteka != sumR){
            while(kopijaFronta.size()>0)
                kopijaFronta.pop();
            while(r.size()>0) {
                kopijaFronta.push(r.front());
                r.pop();
            }
        }
        s.push(kopijaFronta);
    }
}

int main() {
    // Write C++ code here
    std::queue<std::stack<int>> s;
    std::stack<int> stek1;
    stek1.push(1);
    stek1.push(2);
    stek1.push(3);
    
    std::stack<int> stek2;
    stek2.push(2);
    stek2.push(2);
    stek2.push(3);
    
    std::stack<int> stek3;
    stek3.push(4);
    stek3.push(2);
    stek3.push(1);
    
    std::queue<int> redA;
    redA.push(6);
    redA.push(1);
    
    s.push(stek1);
    s.push(stek2);
    s.push(stek3);
    
    premjestiNeparne(s, redA);
    
    int velicinaReda = s.size();
    for(int i=0;i<velicinaReda;i++){
        auto st = s.front();
        s.pop();
        for(int i=0;i<3;i++){
            std::cout << st.top() << " ";
            st.pop();
        }
        
        std::cout << std::endl;
        
    }

    return 0;
}
