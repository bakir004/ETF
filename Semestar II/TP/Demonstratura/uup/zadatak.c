#include <stdio.h>
#include <stdlib.h>

void SpasiNiz(int* niz, int n, char* naziv) {
    FILE* fp = fopen(naziv, "wb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke za pisanje!\n");
        return;
    }
    fwrite(&n, sizeof(int), 1, fp);
    fwrite(niz, sizeof(int), n, fp);
    fclose(fp);
    printf("Niz uspjesno spasen u %s.\n", naziv);
}
int* UcitajNiz(char* naziv, int* n) {
    FILE* fp = fopen(naziv, "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke za citanje!\n");
        return NULL;
    }
    fread(n, sizeof(int), 1, fp);
    int* niz = (int*)malloc((*n) * sizeof(int));
    if (niz == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        fclose(fp);
        return NULL;
    }
    fread(niz, sizeof(int), *n, fp);
    fclose(fp);
    return niz;
}
int main() {
    int n, i;
    char* fajl = "podaci.bin";

    printf("Unesite broj elemenata niza: ");
    scanf("%d", &n);

    int* mojNiz = (int*)malloc(n * sizeof(int));
    
    printf("Unesite %d cijelih brojeva:\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &mojNiz[i]);

    SpasiNiz(mojNiz, n, fajl);
    free(mojNiz);
    printf("Memorija prvog niza oslobodjena.\n\n");

    int noviN, *ucitaniNiz;
    ucitaniNiz = UcitajNiz(fajl, &noviN);

    if (ucitaniNiz != NULL) {
        printf("Ucitani niz iz datoteke (%d elemenata):\n", noviN);
        for (i = 0; i < noviN; i++)
            printf("%d ", ucitaniNiz[i]);
        printf("\n");
        free(ucitaniNiz); 
    }

    return 0;
}



void SpasiNiz(int* niz, int n, char* naziv) {
    FILE* fp = fopen(naziv, "wb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke za pisanje!\n");
        return;
    }
    fwrite(&n, sizeof(int), 1, fp);
    fwrite(niz, sizeof(int), n, fp);
    fclose(fp);
    printf("Niz uspjesno spasen u %s.\n", naziv);
}
// primjer upotrebe: SpasiNiz(niz, 10, "datoteka.dat");































