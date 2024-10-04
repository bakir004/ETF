#include <iostream>
#include <deque>

int Medijan(std::deque<int> dek) {
    for(int i = 0; i < dek.size(); i++) {
        int indexTrenutnogNajmanjeg = -1;
        int trenutniNajmanji = INT_MAX;
        for(int j = 0; j < dek.size(); j++) {
            if(dek[i] < trenutniNajmanji) {
                trenutniNajmanji = dek[i];
                indexTrenutnogNajmanjeg = i;
            }
        }
        if(i == dek.size()/2) return trenutniNajmanji;
        dek[indexTrenutnogNajmanjeg] = INT_MAX;
    }
    return -1;
}

int main() {
    int medijan = Medijan({9,18,8,19,5});
    std::cout << medijan;
    return 0;
}
