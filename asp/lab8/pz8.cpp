#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename T>
void bubble_sort(T* niz, int vel) {
    bool changed = true;
    while(changed) {
        changed = false;
        for(int i = 0; i < vel-1; i++) {
            if(niz[i] > niz[i+1]) {
                std::swap(niz[i], niz[i+1]);
                changed = true;
            }
        }
    }
}

template <typename T>
void selection_sort(T* niz, int vel) {
    for(int i = 0; i < vel; i++) {
        int indexOfMin = i;
        for(int j = i + 1; j < vel; j++)
            if(niz[j] < niz[indexOfMin])
                indexOfMin = j;
        std::swap(niz[i], niz[indexOfMin]);
    }
}

template <typename T>
void quick_sort(T* niz, int vel) {
    if(vel == 1) return;
    if(vel == 2) {
        if(niz[0] > niz[1]) std::swap(niz[0], niz[1]);
        return;
    }
    int pivot = vel-1;
    std::swap(niz[vel/2], niz[vel-1]);
    int i = -1, j = 0;
    while(j != pivot) {
        if(niz[j] < niz[pivot]) {
            i++;
            std::swap(niz[i], niz[j]);
        }
        j++;
    }
    i++;
    std::swap(niz[i], niz[pivot]);
    pivot = i;
    if (pivot > 0)
        quick_sort(niz, pivot);
    if (pivot + 1 < vel)
        quick_sort(niz + pivot + 1, vel - pivot - 1);
}

template <typename T>
void merge_sort(T* niz, int vel) {
    if(vel == 1) return;
    if(vel == 2 && niz[0] > niz[1]) {
        std::swap(niz[0], niz[1]);
        return;
    }
    int half = vel/2;
    merge_sort(niz, half);
    merge_sort(niz + half, (vel%2==1?half+1:half));
    int counterLeft = 0;
    int counterRight = half;
    T* temp = new T[vel];
    for (int i = 0; i < vel; i++) {
        if (counterLeft < half && (counterRight >= vel || niz[counterLeft] <= niz[counterRight]))
            temp[i] = niz[counterLeft++];
        else
            temp[i] = niz[counterRight++];
    }
    for(int i = 0; i < vel; i++)
        niz[i] = temp[i];
    delete[] temp;
}

void ucitaj(std::string filename, int*& niz, int &vel) {
    std::ifstream ulazniTok(filename, std::ios::in);
    std::vector<int> pomocni;
    int n;
    while(ulazniTok >> n)
        pomocni.push_back(n);
    vel = pomocni.size();
    niz = new int[vel];
    for(int i = 0; i < vel; i++)
        niz[i] = pomocni[i];
}

int main() {
    int* niz;
    int n;
    ucitaj("input.txt", niz, n);
    for(int i = 0; i < n; i++)
        std::cout << niz[i] << " ";
    std::cout << '\n';
    quick_sort(niz, n);
    for(int i = 0; i < n; i++)
        std::cout << niz[i] << " ";
    return 0;
}
