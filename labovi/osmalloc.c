#include <windows.h>
#include <stdio.h>
#define heapNew GetProcessHeap()

void *malloc(size_t size){
    if (heapNew)
        return HeapAlloc(heapNew, HEAP_ZERO_MEMORY, size);
    return NULL;
}
void free(void *block) {
    if (block != NULL && heapNew)
        HeapFree(heapNew, 0, block);
}

int main() {
    int *bajti = (int*)malloc(1000);
    int vel = 1000/sizeof(int);
    printf("BROJ INTOVA U 1000 BAJTA JE %d\n\n", vel);
    for(int i = 0; i < vel; i++)
        bajti[i] = 20;
    for(int i = 0; i < vel; i++)
        printf("%d ", bajti[i]);
    return 0;
}

